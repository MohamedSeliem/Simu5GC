/*
 * UeContext.h
 *
 *  Created on: 20 Mar 2025
 *      Author: mseliem
 */

#ifndef UE_CONTEXT_H_
#define UE_CONTEXT_H_

#include <string>
#include <unordered_map>


/// Enum representing UE states in the 5G Core network
enum UeState {
    UNREGISTERED,   // UE has connected but is not yet registered
    REGISTERED,     // UE has successfully registered
    CONNECTED,      // UE has an active session
    IDLE,           // UE is registered but inactive (power-saving mode)
    HANDOVER,       // UE is moving between gNBs
    DEREGISTERED    // UE was registered but is now removed from the network
};


/// Structure to store UE context information
struct UeContext {
    int ueId;               ///< Unique identifier for the UE
    UeState state;          ///< Current UE state
    std::string sliceType;  ///< Network slice associated with the UE
    int sessionId;          ///< Active session ID (if applicable)

    /// Constructor to initialize UE context
    UeContext(int id, UeState initialState, std::string slice, int sessId)
        : ueId(id), state(initialState), sliceType(slice), sessionId(sessId) {}
};

/// Class to manage the UE context table in AMF
class UeContextTable {
  private:
    std::unordered_map<int, UeContext> ueTable;  ///< Storage for UE state information

  public:
    /// Add a new UE to the table
    void addUe(int ueId, UeState state, std::string slice, int sessionId) {
        ueTable[ueId] = UeContext(ueId, state, slice, sessionId);
    }

    /// Update the state of an existing UE
    bool updateUeState(int ueId, UeState newState) {
        if (ueTable.find(ueId) != ueTable.end()) {
            ueTable[ueId].state = newState;
            return true;
        }
        return false;
    }

    /// Retrieve the current state of a UE
    UeState getUeState(int ueId) {
        return ueTable.count(ueId) ? ueTable[ueId].state : UNREGISTERED;
    }

    /// Remove a UE from the context table (for deregistration)
    bool removeUe(int ueId) {
        return ueTable.erase(ueId);
    }

    /// Retrieve the full UE context (if needed)
    UeContext* getUeContext(int ueId) {
        if (ueTable.count(ueId))
            return &ueTable[ueId];
        return nullptr;
    }
};

#endif /* UE_CONTEXT_H_ */
