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

#include "Bsf.h"
#include "BsfMessages.h"
#include <unordered_map>

Define_Module(Bsf);

// Simulated Binding Database
std::unordered_map<int, bool> bindingDatabase;

void Bsf::initialize() {
    EV << "BSF Initialized\n";
}

void Bsf::handleMessage(cMessage *msg) {
    EV << "BSF received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "BINDING_REQUEST") == 0) {  // N19 (SMF → BSF)
        BindingRequest *req = check_and_cast<BindingRequest *>(msg);
        EV << "Processing binding request for UE ID: " << req->ueId << "\n";

        // Simulate binding process
        bindingDatabase[req->ueId] = true;

        BindingResponse *resp = new BindingResponse(req->ueId);
        send(resp, "N19Out", 0);  // Send to SMF via N19
    }
    else if (strcmp(msg->getName(), "BINDING_INFO_REQUEST") == 0) {  // N21 (AMF → BSF)
        BindingInfoRequest *req = check_and_cast<BindingInfoRequest *>(msg);
        EV << "Processing binding info request for UE ID: " << req->ueId << "\n";

        // Check if UE is bound
        bool isBound = bindingDatabase.count(req->ueId) ? bindingDatabase[req->ueId] : false;

        BindingInfoResponse *resp = new BindingInfoResponse(req->ueId, isBound);
        send(resp, "N21Out", 1);  // Send to AMF via N21
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}

