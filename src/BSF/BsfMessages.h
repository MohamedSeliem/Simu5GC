/*
 * BsfMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */

#ifndef BSF_MESSAGES_H_
#define BSF_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

class BindingRequest : public cMessage {
  public:
    int ueId;

    BindingRequest(int id) : cMessage("BINDING_REQUEST") {
        ueId = id;
    }
};

class BindingResponse : public cMessage {
  public:
    int ueId;

    BindingResponse(int id) : cMessage("BINDING_RESPONSE") {
        ueId = id;
    }
};

class BindingInfoRequest : public cMessage {
  public:
    int ueId;

    BindingInfoRequest(int id) : cMessage("BINDING_INFO_REQUEST") {
        ueId = id;
    }
};

class BindingInfoResponse : public cMessage {
  public:
    int ueId;
    bool isBound;

    BindingInfoResponse(int id, bool bound) : cMessage("BINDING_INFO_RESPONSE") {
        ueId = id;
        isBound = bound;
    }
};

#endif /* BSF_MESSAGES_H_ */

