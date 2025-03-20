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

#include "Pcf.h"
#include "PcfMessages.h"

Define_Module(Pcf);

void Pcf::initialize() {
    EV << "PCF Initialized\n";
}

void Pcf::handleMessage(cMessage *msg) {
    EV << "PCF received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "POLICY_REQUEST") == 0) {  // N7 (SMF → PCF)
        PolicyRequest *req = check_and_cast<PolicyRequest *>(msg);
        EV << "Processing policy request for UE ID: " << req->ueId << "\n";

        // Request subscriber policies from UDM via N13
        PolicyDataRequest *policyReq = new PolicyDataRequest(req->ueId);
        send(policyReq, "N13Out");  // Send to UDM via N13
    }
    else if (strcmp(msg->getName(), "POLICY_DATA_RESPONSE") == 0) {  // N13 (UDM → PCF)
        PolicyDataResponse *resp = check_and_cast<PolicyDataResponse *>(msg);
        EV << "Received policy data for UE ID: " << resp->ueId
           << ", Data: " << resp->policyData << "\n";

        // Request charging control from CHF via N15
        ChargingRequest *chargingReq = new ChargingRequest(resp->ueId);
        send(chargingReq, "N15out");  // Send to CHF via N15
    }
    else if (strcmp(msg->getName(), "CHARGING_RESPONSE") == 0) {  // N15 (CHF → PCF)
        ChargingResponse *resp = check_and_cast<ChargingResponse *>(msg);
        EV << "Charging status for UE ID: " << resp->ueId
           << ", Approved: " << resp->approved << "\n";

        // Apply policy decision based on charging and policy data
        std::string policyDecision = resp->approved ? "ALLOW" : "DENY";

        // Send policy decision back to SMF via N7
        PolicyResponse *policyResp = new PolicyResponse(resp->ueId, policyDecision);
        send(policyResp, "N7Out");  // Send to SMF via N7
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
