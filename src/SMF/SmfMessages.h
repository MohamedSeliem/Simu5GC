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

/// Message for modifying an active PDU session (e.g., QoS updates)
class PduSessionModify : public cMessage {
  public:
    int ueId;
    int sessionId;

    PduSessionModify(int id, int sessId) : cMessage("PDU_SESSION_MODIFY") {
        ueId = id;
        sessionId = sessId;
    }
};

/// Message for notifying AMF about session modifications
class PduSessionModifyResponse : public cMessage {
  public:
    int ueId;
    int sessionId;

    PduSessionModifyResponse(int id, int sessId) : cMessage("PDU_SESSION_MODIFY_RESPONSE") {
        ueId = id;
        sessionId = sessId;
    }
};

/// Message for UE deregistration and session release
class UeDeregister : public cMessage {
  public:
    int ueId;

    UeDeregister(int id) : cMessage("UE_DEREGISTER") {
        ueId = id;
    }
};

/// Message for notifying AMF about released sessions
class ReleaseSession : public cMessage {
  public:
    int ueId;

    ReleaseSession(int id) : cMessage("RELEASE_SESSION") {
        ueId = id;
    }
};


#endif /* SMF_MESSAGES_H_ */

