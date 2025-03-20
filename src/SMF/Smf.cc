//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Smf.h"
#include "SmfMessages.h"

Define_Module(Smf);

void Smf::initialize() {
    EV << "SMF Initialized\n";
}

void Smf::handleMessage(cMessage *msg) {
    EV << "SMF received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "PDU_SESSION_REQUEST") == 0) {  // N11 (AMF → SMF)
        PduSessionRequest *req = check_and_cast<PduSessionRequest *>(msg);
        int ueId = req->ueId;
        int sessionId = req->sessionId;
        int sessionKey = ueId * 1000 + sessionId; // Unique session key

        EV << "Processing PDU session request for UE ID: " << ueId
           << ", Session ID: " << sessionId << "\n";

        // Check if the session already exists
        if (sessionTable.getSessionState(ueId, sessionId) == SESSION_ACTIVE) {
            EV << "Warning: UE " << ueId << " already has an active session with ID " << sessionId << ". Ignoring duplicate request.\n";
            delete msg;
            return;
        }

        // Start session establishment
        sessionTable.addSession(ueId, sessionId, SESSION_ESTABLISHING, "eMBB");

        // Request policy control decision from PCF via N7
        PolicyRequest *policyReq = new PolicyRequest(ueId);
        send(policyReq, "N7Out");  // Send to PCF via N7
    }
    else if (strcmp(msg->getName(), "POLICY_RESPONSE") == 0) {  // N7 (PCF → SMF)
        PolicyResponse *resp = check_and_cast<PolicyResponse *>(msg);
        int ueId = resp->ueId;
        int sessionId = resp->sessionId;

        EV << "Received policy decision for UE ID: " << ueId
           << ", Policy: " << resp->policyDecision << "\n";

        // Request session binding from BSF via N19
        BindingRequest *bindingReq = new BindingRequest(ueId);
        send(bindingReq, "N19Out");  // Send to BSF via N19
    }
    else if (strcmp(msg->getName(), "BINDING_RESPONSE") == 0) {  // N19 (BSF → SMF)
        BindingResponse *resp = check_and_cast<BindingResponse *>(msg);
        int ueId = resp->ueId;
        int sessionId = resp->sessionId;

        EV << "Binding successful for UE ID: " << ueId << ", Session ID: " << sessionId
           << ". Setting up UPF session...\n";

        // Update session state before sending request to UPF
        sessionTable.updateSessionState(ueId, sessionId, SESSION_ACTIVE);

        // Setup UPF session via N4
        UpfSessionSetup *upfReq = new UpfSessionSetup(ueId, sessionId);
        send(upfReq, "N4Out");  // Send to UPF via N4
    }
    else if (strcmp(msg->getName(), "UPF_SESSION_RESPONSE") == 0) {  // N4 (UPF → SMF)
        UpfSessionResponse *resp = check_and_cast<UpfSessionResponse *>(msg);
        int ueId = resp->ueId;
        int sessionId = resp->sessionId;

        EV << "UPF session setup " << (resp->success ? "successful" : "failed")
           << " for UE ID: " << ueId << ", Session ID: " << sessionId << "\n";

        if (resp->success) {
            // Notify AMF of successful session setup via N11
            PduSessionAccept *sessionAccept = new PduSessionAccept(ueId, sessionId);
            send(sessionAccept, "N11Out");
        } else {
            EV << "Session setup failed for UE ID: " << ueId << ". Removing session.\n";
            sessionTable.removeSession(ueId, sessionId);
        }
    }
    else if (strcmp(msg->getName(), "PDU_SESSION_MODIFY") == 0) {  // Modify existing session
        PduSessionModify *modify = check_and_cast<PduSessionModify *>(msg);
        int ueId = modify->ueId;
        int sessionId = modify->sessionId;

        if (sessionTable.getSessionState(ueId, sessionId) == SESSION_ACTIVE) {
            EV << "Modifying session for UE " << ueId << ", Session ID: " << sessionId << ". Transitioning to MODIFIED state.\n";
            sessionTable.updateSessionState(ueId, sessionId, SESSION_MODIFIED);

            // Send session modification response
            PduSessionModifyResponse *modifyResp = new PduSessionModifyResponse(ueId, sessionId);
            send(modifyResp, "N11Out");
        } else {
            EV << "Error: Cannot modify session for UE " << ueId << ". No active session found.\n";
        }
    }
    else if (strcmp(msg->getName(), "UE_DEREGISTER") == 0) {  // Release all sessions for UE
        UeDeregister *dereg = check_and_cast<UeDeregister *>(msg);
        int ueId = dereg->ueId;

        EV << "Releasing all sessions for UE " << ueId << ".\n";

        // Iterate through all sessions for this UE and release them
        for (auto it = sessionTable.sessionTable.begin(); it != sessionTable.sessionTable.end();) {
            if (it->second.ueId == ueId) {
                int sessionId = it->second.sessionId;
                EV << "Releasing session " << sessionId << " for UE " << ueId << ".\n";
                sessionTable.updateSessionState(ueId, sessionId, SESSION_RELEASED);
                it = sessionTable.sessionTable.erase(it);
            } else {
                ++it;
            }
        }

        // Notify network functions
        ReleaseSession *releaseSession = new ReleaseSession(ueId);
        send(releaseSession, "N11Out");
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
