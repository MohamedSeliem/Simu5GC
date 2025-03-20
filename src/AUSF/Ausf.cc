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

#include "Ausf.h"

Define_Module(Ausf);

#include "Ausf.h"
#include "AusfMessages.h"


void Ausf::initialize() {
    EV << "AUSF Initialized\n";
}

void Ausf::handleMessage(cMessage *msg) {
    EV << "AUSF received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "AUTH_REQUEST") == 0) {  // N8 (AMF → AUSF)
        AuthRequest *req = check_and_cast<AuthRequest *>(msg);
        EV << "Received authentication request for UE ID: " << req->ueId << "\n";

        // Request authentication data from UDM via N10
        AuthDataRequest *dataReq = new AuthDataRequest(req->ueId);
        send(dataReq, "N10Out");  // Send to UDM via N10
    }
    else if (strcmp(msg->getName(), "AUTH_DATA_RESPONSE") == 0) {  // N10 (UDM → AUSF)
        AuthDataResponse *resp = check_and_cast<AuthDataResponse *>(msg);
        EV << "Received authentication vector for UE ID: " << resp->ueId << "\n";

        // Simulate authentication check (pass/fail)
        bool authResult = (resp->authVector == "valid_vector");

        // Send authentication response to AMF via N8
        AuthResponse *authResp = new AuthResponse(resp->ueId, authResult);
        send(authResp, "N8Out");

        // Store authentication result in UDR via N20
        AuthStorageRequest *storageReq = new AuthStorageRequest(resp->ueId, authResult ? "SUCCESS" : "FAILURE");
        send(storageReq, "N20Out");  // Send to UDR via N20
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
