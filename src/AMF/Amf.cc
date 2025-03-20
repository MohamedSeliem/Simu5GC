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

#include "Amf.h"

Define_Module(Amf);

void Amf::initialize()
{
    EV << "AMF Initialized\n";
}

void Amf::handleMessage(cMessage *msg) {
    EV << "AMF received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "UE_REGISTER") == 0) {  // N1 (UE Registration)
        UeRegister *reg = check_and_cast<UeRegister *>(msg);
        int ueId = reg->ueId;
        std::string sliceType = reg->requestedSlice;

        EV << "Processing UE Registration: UE ID = " << ueId
           << ", Requested Slice = " << sliceType << "\n";

        // If UE is deregistered, reset it to UNREGISTERED before registration
        if (ueTable.getUeState(ueId) == DEREGISTERED) {
            EV << "UE ID " << ueId << " was previously deregistered. Resetting to UNREGISTERED before re-registration.\n";
            ueTable.updateUeState(ueId, UNREGISTERED);
        }

        // Proceed with registration for new or reset UEs
        if (ueTable.getUeState(ueId) == UNREGISTERED) {
            EV << "Processing UE Registration: UE ID = " << ueId
               << ", Requested Slice = " << sliceType << "\n";
            ueTable.addUe(ueId, REGISTERED, sliceType, ueId * 10); // Assign session ID dynamically

            // Request slice selection from NSSF via N17
            SliceSelectionRequest *sliceRequest = new SliceSelectionRequest(ueId, sliceType);
            send(sliceRequest, "N17Out");
        } else {
            EV << "Warning: UE ID " << ueId << " is already registered. Ignoring duplicate registration request.\n";
        }
    }
    else if (strcmp(msg->getName(), "SLICE_SELECTION_RESPONSE") == 0) {  // N17 (Slice Selection)
        SliceSelectionResponse *resp = check_and_cast<SliceSelectionResponse *>(msg);
        int ueId = resp->ueId;

        EV << "Received slice selection for UE ID " << ueId << ". Proceeding with authentication...\n";

        // Send authentication request to AUSF via N8
        AuthRequest *authReq = new AuthRequest(ueId);
        send(authReq, "N8Out");
    }
    else if (strcmp(msg->getName(), "AUTH_RESPONSE") == 0) {  // N8 (Authentication)
        AuthResponse *auth = check_and_cast<AuthResponse *>(msg);
        int ueId = auth->ueId;

        if (auth->isAuthenticated) {
            EV << "UE ID " << ueId << " authenticated. Transitioning to CONNECTED state.\n";
            ueTable.updateUeState(ueId, CONNECTED);

            // Send binding request to BSF via N21
            BindingRequest *bindingRequest = new BindingRequest(ueId);
            send(bindingRequest, "N21Out");
        } else {
            EV << "UE ID " << ueId << " authentication failed. Transitioning to DEREGISTERED.\n";
            ueTable.updateUeState(ueId, DEREGISTERED);
        }
    }
    else if (strcmp(msg->getName(), "BINDING_RESPONSE") == 0) {  // N21 (Binding Support)
        BindingResponse *binding = check_and_cast<BindingResponse *>(msg);
        int ueId = binding->ueId;

        EV << "Binding successful for UE ID: " << ueId << ". Sending PDU session request to SMF...\n";

        // Ensure UE is in the correct state before proceeding
        if (ueTable.getUeState(ueId) == CONNECTED) {
            PduSessionRequest *sessionReq = new PduSessionRequest(ueId, ueId * 10);
            send(sessionReq, "N11Out");
        } else {
            EV << "Error: UE ID " << ueId << " is not in the CONNECTED state. Cannot proceed with PDU session setup.\n";
        }
    }
    else if (strcmp(msg->getName(), "PDU_SESSION_ACCEPT") == 0) {  // N11 (PDU session established)
        PduSessionAccept *session = check_and_cast<PduSessionAccept *>(msg);
        int ueId = session->ueId;

        EV << "PDU Session established for UE ID " << ueId << " Session ID: " << session->sessionId << "\n";
        ueTable.updateUeState(ueId, CONNECTED);

        // Send UE registration complete message
        cMessage *regComplete = new cMessage("UE_REGISTRATION_COMPLETE");
        send(regComplete, "N1Out");
    }
    else if (strcmp(msg->getName(), "HANDOVER_REQUEST") == 0) {  // N2/N12 (Handover Request from gNB)
        HandoverRequest *handover = check_and_cast<HandoverRequest *>(msg);
        int ueId = handover->ueId;

        if (ueTable.getUeState(ueId) == CONNECTED) {
            EV << "Handling mobility event (handover) for UE ID " << ueId
               << " to target gNB ID " << handover->targetGnbId << "\n";

            // Transition UE state to HANDOVER
            ueTable.updateUeState(ueId, HANDOVER);

            // Inform target gNB about incoming UE handover
            HandoverConfirm *handoverConfirm = new HandoverConfirm(ueId);
            send(handoverConfirm, "N12Out");
        } else {
            EV << "Error: UE ID " << ueId << " is not in CONNECTED state. Cannot process handover.\n";
        }
    }
    else if (strcmp(msg->getName(), "AMF_TRANSFER") == 0) {  // N14 (Inter-AMF Handover)
        AmfTransfer *amfTransfer = check_and_cast<AmfTransfer *>(msg);
        int ueId = amfTransfer->ueId;

        EV << "Handling inter-AMF handover for UE ID " << ueId
           << " to target AMF ID " << amfTransfer->targetAmfId << "\n";

        // Transition UE state to HANDOVER
        ueTable.updateUeState(ueId, HANDOVER);

        // Transfer UE context to another AMF
        AmfTransferComplete *handoverComplete = new AmfTransferComplete(ueId);
        send(handoverComplete, "N14Out");
    }
    else if (strcmp(msg->getName(), "PAGING_REQUEST") == 0) {  // N2 (Paging from gNB)
        PagingRequest *paging = check_and_cast<PagingRequest *>(msg);
        int ueId = paging->ueId;

        EV << "Received paging request for UE ID: " << ueId << ". Notifying UE...\n";

        // Forward paging message to UE
        PagingNotification *pagingNotify = new PagingNotification(ueId);
        send(pagingNotify, "N1Out");
    }
    else if (strcmp(msg->getName(), "UE_DEREGISTER") == 0) {  // UE Deregistration
        UeDeregister *dereg = check_and_cast<UeDeregister *>(msg);
        int ueId = dereg->ueId;

        EV << "UE ID " << ueId << " is de-registering. Releasing resources...\n";

        // Update UE state and remove from context
        ueTable.updateUeState(ueId, DEREGISTERED);
        ueTable.removeUe(ueId);

        // Notify SMF to remove session
        ReleaseSession *releaseSession = new ReleaseSession(ueId);
        send(releaseSession, "N11Out");

        // Notify network functions
        UeContextRelease *notifyRelease = new UeContextRelease(ueId);
        send(notifyRelease, "N2Out");
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
