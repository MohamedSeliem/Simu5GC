/*
 * AusfMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */

#ifndef AUSF_MESSAGES_H_
#define AUSF_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

class AuthRequest : public cMessage {
  public:
    int ueId;

    AuthRequest(int id) : cMessage("AUTH_REQUEST") {
        ueId = id;
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

class AuthResponse : public cMessage {
  public:
    int ueId;
    bool isAuthenticated;

    AuthResponse(int id, bool auth) : cMessage("AUTH_RESPONSE") {
        ueId = id;
        isAuthenticated = auth;
    }
};

class AuthStorageRequest : public cMessage {
  public:
    int ueId;
    std::string authResult;

    AuthStorageRequest(int id, std::string result) : cMessage("AUTH_STORAGE_REQUEST") {
        ueId = id;
        authResult = result;
    }
};

#endif /* AUSF_MESSAGES_H_ */


