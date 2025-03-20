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

#include "Nssf.h"
#include <unordered_map>

Define_Module(Nssf);

// Define available slices and their priority
std::unordered_map<std::string, std::string> sliceMapping = {
    {"eMBB", "Slice_A"},  // Enhanced Mobile Broadband → Slice_A
    {"URLLC", "Slice_B"}, // Ultra-Reliable Low-Latency → Slice_B
    {"mMTC", "Slice_C"}   // Massive IoT → Slice_C
};

// Slice Availability (Simulated Load)
std::unordered_map<std::string, bool> sliceAvailability = {
    {"Slice_A", true},
    {"Slice_B", true},
    {"Slice_C", true},
    {"Slice_Default", true}  // **Ensure a default slice is always available**
};

void Nssf::initialize()
{
    EV << "NSSF Initialized\n";
}

void Nssf::handleMessage(cMessage *msg)
{
    EV << "NSSF received message: " << msg->getName() << " from "
           << msg->getSenderModule()->getName() << "\n";

    if (strcmp(msg->getName(), "SLICE_SELECTION_REQUEST") == 0) {
            SliceSelectionRequest *req = check_and_cast<SliceSelectionRequest *>(msg);
            EV << "Processing Slice Selection for UE ID: " << req->ueId
               << ", Requested Slice: " << req->requestedSlice << "\n";

            std::string allocatedSlice;

            // Check if requested slice type exists**
            if (sliceMapping.find(req->requestedSlice) != sliceMapping.end()) {
                allocatedSlice = sliceMapping[req->requestedSlice];

                // Check if slice is available**
                if (!sliceAvailability[allocatedSlice]) {
                    EV << "Requested slice " << allocatedSlice << " is congested. Finding alternative...\n";

                    // Find an alternative slice**
                    bool alternativeFound = false;
                    for (const auto& slice : sliceAvailability) {
                        if (slice.second) {  // Check if slice is available
                            allocatedSlice = slice.first;
                            alternativeFound = true;
                            EV << "Alternative slice allocated: " << allocatedSlice << "\n";
                            break;
                        }
                    }

                    // If no alternative is found, assign `Slice_Default`**
                    if (!alternativeFound) {
                        EV << "No available slices! Assigning Slice_Default.\n";
                        allocatedSlice = "Slice_Default";
                    }
                }
            } else {
                EV << "Unknown slice type requested. Assigning Slice_Default.\n";
                allocatedSlice = "Slice_Default";  // Assign default slice if unknown
            }


            SliceSelectionResponse *resp = new SliceSelectionResponse(req->ueId, allocatedSlice);
            send(resp, "N17Out");  // Send response to AMF via N17
        }

    else {
            EV << "Unknown message received: " << msg->getName() << "\n";
        }

        delete msg; // Cleanup after processing
}
