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
        EV << "Processing PDU session request for UE ID: " << req->ueId
           << ", Session ID: " << req->sessionId << "\n";

        // Request policy control decision from PCF via N7
        PolicyRequest *policyReq = new PolicyRequest(req->ueId);
        send(policyReq, "N7Out");  // Send to PCF via N7
    }
    else if (strcmp(msg->getName(), "POLICY_RESPONSE") == 0) {  // N7 (PCF → SMF)
        PolicyResponse *resp = check_and_cast<PolicyResponse *>(msg);
        EV << "Received policy decision for UE ID: " << resp->ueId
           << ", Policy: " << resp->policyDecision << "\n";

        // Request session binding from BSF via N19
        BindingRequest *bindingReq = new BindingRequest(resp->ueId);
        send(bindingReq, "N19Out");  // Send to BSF via N19
    }
    else if (strcmp(msg->getName(), "BINDING_RESPONSE") == 0) {  // N19 (BSF → SMF)
        BindingResponse *resp = check_and_cast<BindingResponse *>(msg);
        EV << "Binding successful for UE ID: " << resp->ueId << ". Setting up UPF session...\n";

        // Setup UPF session via N4
        UpfSessionSetup *upfReq = new UpfSessionSetup(resp->ueId, 1);
        send(upfReq, "N4Out");  // Send to UPF via N4
    }
    else if (strcmp(msg->getName(), "UPF_SESSION_RESPONSE") == 0) {  // N4 (UPF → SMF)
        UpfSessionResponse *resp = check_and_cast<UpfSessionResponse *>(msg);
        EV << "UPF session setup " << (resp->success ? "successful" : "failed")
           << " for UE ID: " << resp->ueId << "\n";

        if (resp->success) {
            // Notify AMF of successful session setup via N11
            PduSessionAccept *sessionAccept = new PduSessionAccept(resp->ueId, resp->sessionId);
            send(sessionAccept, "N11Out");
        }
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
