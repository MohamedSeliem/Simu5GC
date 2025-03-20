/*
 * UdmMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */

#ifndef UDM_MESSAGES_H_
#define UDM_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

class AuthDataRequest : public cMessage {
  public:
    int ueId;

    AuthDataRequest(int id) : cMessage("AUTH_DATA_REQUEST") {
        ueId = id;
    }
};

class AuthDataResponse : public cMessage {
  public:
    int ueId;
    std::string authVector;

    AuthDataResponse(int id, std::string vector) : cMessage("AUTH_DATA_RESPONSE") {
        ueId = id;
        authVector = vector;
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

class SubscriptionProfileRequest : public cMessage {
  public:
    int ueId;

    SubscriptionProfileRequest(int id) : cMessage("SUBSCRIPTION_PROFILE_REQUEST") {
        ueId = id;
    }
};

class SubscriptionProfileResponse : public cMessage {
  public:
    int ueId;
    std::string profileData;

    SubscriptionProfileResponse(int id, std::string profile) : cMessage("SUBSCRIPTION_PROFILE_RESPONSE") {
        ueId = id;
        profileData = profile;
    }
};

#endif /* UDM_MESSAGES_H_ */

