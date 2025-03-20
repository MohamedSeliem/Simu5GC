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

#include "Udr.h"
#include "UdrMessages.h"
#include <unordered_map>

Define_Module(Udr);

// Simulated Database for UDR
std::unordered_map<int, std::string> subscriptionDatabase = {
    {1001, "Premium"},
    {1002, "Standard"},
    {1003, "Basic"}
};

std::unordered_map<int, std::string> authDatabase = {
    {1001, "valid_vector"},
    {1002, "valid_vector"},
    {1003, "invalid_vector"}
};

void Udr::initialize() {
    EV << "UDR Initialized\n";
}

void Udr::handleMessage(cMessage *msg) {
    EV << "UDR received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "SUBSCRIPTION_PROFILE_REQUEST") == 0) {  // N18 (UDM → UDR)
        SubscriptionProfileRequest *req = check_and_cast<SubscriptionProfileRequest *>(msg);
        EV << "Processing subscription profile request for UE ID: " << req->ueId << "\n";

        std::string profileData = subscriptionDatabase.count(req->ueId) ? subscriptionDatabase[req->ueId] : "Unknown";

        SubscriptionProfileResponse *resp = new SubscriptionProfileResponse(req->ueId, profileData);
        send(resp, "out", 0);  // Send to UDM via N18
    }
    else if (strcmp(msg->getName(), "AUTH_DATA_REQUEST") == 0) {  // N20 (AUSF → UDR)
        AuthDataRequest *req = check_and_cast<AuthDataRequest *>(msg);
        EV << "Processing authentication data request for UE ID: " << req->ueId << "\n";

        std::string authVector = authDatabase.count(req->ueId) ? authDatabase[req->ueId] : "invalid_vector";

        AuthDataResponse *resp = new AuthDataResponse(req->ueId, authVector);
        send(resp, "out", 1);  // Send to AUSF via N20
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}

