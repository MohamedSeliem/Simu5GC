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
        // Extract UE ID and requested slice type
        UeRegister *reg = check_and_cast<UeRegister *>(msg);
        int ueId = reg->ueId;
        std::string sliceType = reg->requestedSlice;

        EV << "Processing UE Registration: UE ID = " << ueId
                   << ", Requested Slice = " << sliceType << "\n";

        // Request slice selection from NSSF via N17
        SliceSelectionRequest *sliceRequest = new SliceSelectionRequest(ueId, sliceType);
        send(sliceRequest, "N17Out");
    }
    else if (strcmp(msg->getName(), "SLICE_SELECTION_RESPONSE") == 0) {  // N17 (Slice Selection)
        SliceSelectionResponse *resp = check_and_cast<SliceSelectionResponse *>(msg);

        EV << "Received slice selection for UE ID " << resp->ueId << ". Proceeding with authentication...\n";
        // Send authentication request to AUSF via N8
        AuthRequest *authReq = new AuthRequest(resp->ueId);
        send(authReq, "N8Out");
    }
    else if (strcmp(msg->getName(), "AUTH_RESPONSE") == 0) {  // N8 (Authentication)
        AuthResponse *auth = check_and_cast<AuthResponse *>(msg);
        if (auth->isAuthenticated) {
            EV << "UE ID " << auth->ueId << " authenticated. Initiating PDU session setup...\n";

            // Send binding request to BSF via N21
            BindingRequest *bindingRequest = new BindingRequest(auth->ueId);
            send(bindingRequest, "N21Out");
        } else {
            EV << "UE ID " << auth->ueId << " authentication failed.\n";
            //todo: create message for failed authentication.
        }
    }
    else if (strcmp(msg->getName(), "BINDING_RESPONSE") == 0) {  // N21 (Binding Support)
        BindingResponse *binding = check_and_cast<BindingResponse *>(msg);
        EV << "Binding successful for UE ID: " << binding->ueId << ". Sending PDU session request to SMF...\n";

        // Send PDU session setup request to SMF via N11
        PduSessionRequest *sessionReq = new PduSessionRequest(binding->ueId, 1);
        send(sessionReq, "N11Out");
    }
    else if (strcmp(msg->getName(), "PDU_SESSION_ACCEPT") == 0) {  // N11 (PDU session established)
        PduSessionAccept *session = check_and_cast<PduSessionAccept *>(msg);
        EV << "PDU Session established for UE ID " << session->ueId << " Session ID: " << session->sessionId << "\n";

        // Send UE registration complete message
        cMessage *regComplete = new cMessage("UE_REGISTRATION_COMPLETE");
        send(regComplete, "N1Out");
    }
    else if (strcmp(msg->getName(), "HANDOVER_REQUEST") == 0) {  // N2/N12 (Handover Request from gNB)
        HandoverRequest *handover = check_and_cast<HandoverRequest *>(msg);
        EV << "Handling mobility event (handover) for UE ID " << handover->ueId
           << " to target gNB ID " << handover->targetGnbId << "\n";

        // Inform target gNB about incoming UE handover
        HandoverConfirm *handoverConfirm = new HandoverConfirm(handover->ueId);
        send(handoverConfirm, "N12Out");
    }
    else if (strcmp(msg->getName(), "AMF_TRANSFER") == 0) {  // N14 (Inter-AMF Handover)
        AmfTransfer *amfTransfer = check_and_cast<AmfTransfer *>(msg);
        EV << "Handling inter-AMF handover for UE ID " << amfTransfer->ueId
           << " to target AMF ID " << amfTransfer->targetAmfId << "\n";

        // Transfer UE context to another AMF
        AmfTransferComplete *handoverComplete = new AmfTransferComplete(amfTransfer->ueId);
        send(handoverComplete, "N14Out");
    }
    else if (strcmp(msg->getName(), "PAGING_REQUEST") == 0) {  // N2 (Paging from gNB)
        PagingRequest *paging = check_and_cast<PagingRequest *>(msg);
        EV << "Received paging request for UE ID: " << paging->ueId << ". Notifying UE...\n";

        // Forward paging message to UE
        PagingNotification *pagingNotify = new PagingNotification(paging->ueId);
        send(pagingNotify, "N1Out");
    }
    else if (strcmp(msg->getName(), "UE_DEREGISTER") == 0) {  // UE Deregistration
        UeDeregister *dereg = check_and_cast<UeDeregister *>(msg);
        EV << "UE ID " << dereg->ueId << " is de-registering. Releasing resources...\n";

        // Notify SMF to remove session
        ReleaseSession *releaseSession = new ReleaseSession(dereg->ueId);
        send(releaseSession, "N11Out");

        // Notify network functions
        UeContextRelease *notifyRelease = new UeContextRelease(dereg->ueId);
        send(notifyRelease, "N2Out");
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
