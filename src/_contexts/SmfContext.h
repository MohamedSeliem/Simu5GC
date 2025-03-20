/*
 * SmfContext.h
 *
 *  Created on: 20 Mar 2025
 *      Author: mseliem
 */

#ifndef SMF_CONTEXT_H_
#define SMF_CONTEXT_H_

#include <unordered_map>

/// Enum representing session states in SMF
enum SessionState {
    SESSION_INACTIVE,
    SESSION_ESTABLISHING,
    SESSION_ACTIVE,
    SESSION_MODIFIED,
    SESSION_RELEASED
};

/// Structure to store session context information
struct SessionContext {
    int ueId;               ///< Unique UE identifier
    int sessionId;          ///< Unique session identifier
    SessionState state;     ///< Session state
    std::string sliceType;  ///< Network slice for the session

    /// Constructor to initialize session context
    SessionContext(int id, int sessId, SessionState initialState, std::string slice)
        : ueId(id), sessionId(sessId), state(initialState), sliceType(slice) {}
};

/// Class to manage the session context table in SMF
class SmfContextTable {
  private:
    std::unordered_map<int, SessionContext> sessionTable;  ///< Tracks multiple sessions per UE

  public:
    /// Add a new session
    void addSession(int ueId, int sessionId, SessionState state, std::string slice) {
        int sessionKey = ueId * 1000 + sessionId; // Unique key for multi-session tracking
        sessionTable[sessionKey] = SessionContext(ueId, sessionId, state, slice);
    }

    /// Update session state
    bool updateSessionState(int ueId, int sessionId, SessionState newState) {
        int sessionKey = ueId * 1000 + sessionId;
        if (sessionTable.find(sessionKey) != sessionTable.end()) {
            sessionTable[sessionKey].state = newState;
            return true;
        }
        return false;
    }

    /// Get session state
    SessionState getSessionState(int ueId, int sessionId) {
        int sessionKey = ueId * 1000 + sessionId;
        return sessionTable.count(sessionKey) ? sessionTable[sessionKey].state : SESSION_INACTIVE;
    }

    /// Remove session
    bool removeSession(int ueId, int sessionId) {
        int sessionKey = ueId * 1000 + sessionId;
        return sessionTable.erase(sessionKey);
    }

    /// Retrieve the full session context (if needed)
    SessionContext* getSessionContext(int ueId) {
        if (sessionTable.count(ueId))
            return &sessionTable[ueId];
        return nullptr;
    }

};

#endif /* SMF_CONTEXT_H_ */

