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

#ifndef AMF_MESSAGES_H_
#define AMF_MESSAGES_H_

#include <omnetpp.h>

using namespace omnetpp;

class UeRegister : public cMessage {
  public:
    int ueId;
    std::string requestedSlice;  // Slice type: eMBB, URLLC, mMTC

    UeRegister(int id, std::string slice) : cMessage("UE_REGISTER") {
        ueId = id;
        requestedSlice = slice;
    }
};

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

class AuthRequest : public cMessage {
  public:
    int ueId;

    AuthRequest(int id) : cMessage("AUTH_REQUEST") {
        ueId = id;
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

class HandoverRequest : public cMessage {
  public:
    int ueId;
    int targetGnbId;

    HandoverRequest(int id, int targetId) : cMessage("HANDOVER_REQUEST") {
        ueId = id;
        targetGnbId = targetId;
    }
};

class HandoverConfirm : public cMessage {
  public:
    int ueId;

    HandoverConfirm(int id) : cMessage("HANDOVER_CONFIRM") {
        ueId = id;
    }
};

class AmfTransfer : public cMessage {
  public:
    int ueId;
    int targetAmfId;

    AmfTransfer(int id, int targetId) : cMessage("AMF_TRANSFER") {
        ueId = id;
        targetAmfId = targetId;
    }
};

class AmfTransferComplete : public cMessage {
  public:
    int ueId;

    AmfTransferComplete(int id) : cMessage("HANDOVER_COMPLETE") {
        ueId = id;
    }
};

class PagingRequest : public cMessage {
  public:
    int ueId;

    PagingRequest(int id) : cMessage("PAGING_REQUEST") {
        ueId = id;
    }
};

class PagingNotification : public cMessage {
  public:
    int ueId;

    PagingNotification(int id) : cMessage("PAGING_NOTIFICATION") {
        ueId = id;
    }
};

class UeDeregister : public cMessage {
  public:
    int ueId;

    UeDeregister(int id) : cMessage("UE_DEREGISTER") {
        ueId = id;
    }
};

class ReleaseSession : public cMessage {
  public:
    int ueId;

    ReleaseSession(int id) : cMessage("RELEASE_SESSION") {
        ueId = id;
    }
};

class UeContextRelease : public cMessage {
  public:
    int ueId;

    UeContextRelease(int id) : cMessage("UE_CONTEXT_RELEASE") {
        ueId = id;
    }
};

#endif /* AMF_MESSAGES_H_ */
