/*
 * PcfMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */

#ifndef PCF_MESSAGES_H_
#define PCF_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

class PolicyRequest : public cMessage {
  public:
    int ueId;

    PolicyRequest(int id) : cMessage("POLICY_REQUEST") {
        ueId = id;
    }
};

class PolicyDataRequest : public cMessage {
  public:
    int ueId;

    PolicyDataRequest(int id) : cMessage("POLICY_DATA_REQUEST") {
        ueId = id;
    }
};

class PolicyDataResponse : public cMessage {
  public:
    int ueId;
    std::string policyData;

    PolicyDataResponse(int id, std::string data) : cMessage("POLICY_DATA_RESPONSE") {
        ueId = id;
        policyData = data;
    }
};

class ChargingRequest : public cMessage {
  public:
    int ueId;

    ChargingRequest(int id) : cMessage("CHARGING_REQUEST") {
        ueId = id;
    }
};

class ChargingResponse : public cMessage {
  public:
    int ueId;
    bool approved;

    ChargingResponse(int id, bool status) : cMessage("CHARGING_RESPONSE") {
        ueId = id;
        approved = status;
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

#endif /* PCF_MESSAGES_H_ */

