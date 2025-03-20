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

#include "Udm.h"
#include "UdmMessages.h"

Define_Module(Udm);

void Udm::initialize() {
    EV << "UDM Initialized\n";
}

void Udm::handleMessage(cMessage *msg) {
    EV << "UDM received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "AUTH_DATA_REQUEST") == 0) {  // N10 (AUSF → UDM)
        AuthDataRequest *req = check_and_cast<AuthDataRequest *>(msg);
        EV << "Processing authentication request for UE ID: " << req->ueId << "\n";

        // Simulate authentication vector retrieval
        std::string authVector = "valid_vector";

        AuthDataResponse *resp = new AuthDataResponse(req->ueId, authVector);
        send(resp, "N10Out");  // Send to AUSF via N10
    }
    else if (strcmp(msg->getName(), "POLICY_DATA_REQUEST") == 0) {  // N13 (PCF → UDM)
        PolicyDataRequest *req = check_and_cast<PolicyDataRequest *>(msg);
        EV << "Processing policy data request for UE ID: " << req->ueId << "\n";

        // Simulate requesting profile from UDR via N18
        SubscriptionProfileRequest *profileReq = new SubscriptionProfileRequest(req->ueId);
        send(profileReq, "N18Out");  // Send to UDR via N18
    }
    else if (strcmp(msg->getName(), "SUBSCRIPTION_PROFILE_RESPONSE") == 0) {  // N18 (UDR → UDM)
        SubscriptionProfileResponse *resp = check_and_cast<SubscriptionProfileResponse *>(msg);
        EV << "Received subscription profile for UE ID: " << resp->ueId
           << ", Profile: " << resp->profileData << "\n";

        // Send policy data response to PCF via N13
        PolicyDataResponse *policyResp = new PolicyDataResponse(resp->ueId, resp->profileData);
        send(policyResp, "N13Out", 1);  // Send to PCF via N13
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
