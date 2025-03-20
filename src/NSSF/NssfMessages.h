/*
 * NssfMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */

#ifndef NSSF_MESSAGES_H_
#define NSSF_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

class SliceSelectionRequest : public cMessage {
  public:
    int ueId;
    std::string requestedSlice;

    SliceSelectionRequest(int id, std::string slice) : cMessage("SLICE_SELECTION_REQUEST") {
        ueId = id;
        requestedSlice = slice;
    }
};

class SliceSelectionResponse : public cMessage {
  public:
    int ueId;
    std::string allocatedSlice;

    SliceSelectionResponse(int id, std::string slice) : cMessage("SLICE_SELECTION_RESPONSE") {
        ueId = id;
        allocatedSlice = slice;
    }
};

#endif /* NSSF_MESSAGES_H_ */

