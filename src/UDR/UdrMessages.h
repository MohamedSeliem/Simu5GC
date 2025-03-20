/*
 * UdrMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */

#ifndef UDR_MESSAGES_H_
#define UDR_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

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

#endif /* UDR_MESSAGES_H_ */

