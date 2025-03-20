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

#include "Upf.h"
#include "UpfMessages.h"
#include <unordered_map>

Define_Module(Upf);

// Simulated Session Table
std::unordered_map<int, int> sessionTable;

void Upf::initialize() {
    EV << "UPF Initialized\n";
}

void Upf::handleMessage(cMessage *msg) {
    EV << "UPF received message: " << msg->getName() << " from "
       << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "UPF_SESSION_SETUP") == 0) {  // N4 (SMF → UPF)
        UpfSessionSetup *req = check_and_cast<UpfSessionSetup *>(msg);
        EV << "Setting up session for UE ID: " << req->ueId << ", Session ID: " << req->sessionId << "\n";

        sessionTable[req->ueId] = req->sessionId;

        UpfSessionResponse *resp = new UpfSessionResponse(req->ueId, req->sessionId, true);
        send(resp, "N4Out");  // Send to SMF via N4
    }
    else if (strcmp(msg->getName(), "DATA_PACKET") == 0) {  // N3 (gNB → UPF)
        DataPacket *packet = check_and_cast<DataPacket *>(msg);
        EV << "Received data packet from UE ID: " << packet->ueId << " Payload: " << packet->payload << "\n";

        // Forward data to DN via N6
        DataPacket *dnPacket = new DataPacket(packet->ueId, packet->payload);
        send(dnPacket, "N6Out");  // Send to DN via N6
    }
    else if (strcmp(msg->getName(), "UPF_HANDOVER") == 0) {  // N9 (UPF → UPF)
        UpfHandover *handover = check_and_cast<UpfHandover *>(msg);
        EV << "Handling UPF handover for UE ID: " << handover->ueId
           << " to target UPF ID: " << handover->targetUpfId << "\n";

        // Forward handover request to target UPF via N9
        UpfHandover *handoverMsg = new UpfHandover(handover->ueId, handover->targetUpfId);
        send(handoverMsg, "N9Out");  // Send to target UPF via N9
    }
    else {
        EV << "Unknown message received: " << msg->getName() << "\n";
    }

    delete msg; // Cleanup after processing
}
