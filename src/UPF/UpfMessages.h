/*
 * UpfMessages.h
 *
 *  Created on: 19 Mar 2025
 *      Author: mseliem
 */
#ifndef UPF_MESSAGES_H_
#define UPF_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

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

class DataPacket : public cMessage {
  public:
    int ueId;
    std::string payload;

    DataPacket(int id, std::string data) : cMessage("DATA_PACKET") {
        ueId = id;
        payload = data;
    }
};

class UpfHandover : public cMessage {
  public:
    int ueId;
    int targetUpfId;

    UpfHandover(int id, int targetId) : cMessage("UPF_HANDOVER") {
        ueId = id;
        targetUpfId = targetId;
    }
};

#endif /* UPF_MESSAGES_H_ */

