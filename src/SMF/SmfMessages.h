/*
 * SmfMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */

#ifndef SMF_MESSAGES_H_
#define SMF_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

class PduSessionRequest : public cMessage {
  public:
    int ueId;
    int sessionId;

    PduSessionRequest(int id, int sessId) : cMessage("PDU_SESSION_REQUEST") {
        ueId = id;
        sessionId = sessId;
    }
};

class PduSessionAccept : public cMessage {
  public:
    int ueId;
    int sessionId;

    PduSessionAccept(int id, int sessId) : cMessage("PDU_SESSION_ACCEPT") {
        ueId = id;
        sessionId = sessId;
    }
};

class PolicyRequest : public cMessage {
  public:
    int ueId;

    PolicyRequest(int id) : cMessage("POLICY_REQUEST") {
        ueId = id;
    }
};

class PolicyResponse : public cMessage {
  public:
    int ueId;
    std::string policyDecision;

    PolicyResponse(int id, std::string decision) : cMessage("POLICY_RESPONSE") {
        ueId = id;
        policyDecision = decision;
    }
};

class UpfSessionSetup : public cMessage {
  public:
    int ueId;
    int sessionId;

    UpfSessionSetup(int id, int sessId) : cMessage("UPF_SESSION_SETUP") {
        ueId = id;
        sessionId = sessId;
    }
};

class UpfSessionResponse : public cMessage {
  public:
    int ueId;
    int sessionId;
    bool success;

    UpfSessionResponse(int id, int sessId, bool res) : cMessage("UPF_SESSION_RESPONSE") {
        ueId = id;
        sessionId = sessId;
        success = res;
    }
};

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

#endif /* SMF_MESSAGES_H_ */

