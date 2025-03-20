/*
 * UpfSessionTable.h
 *
 *  Created on: 20 Mar 2025
 *      Author: mseliem
 */

#ifndef CONTEXTS_UPFSESSIONTABLE_H_
#define CONTEXTS_UPFSESSIONTABLE_H_


#include <unordered_map>

/// Structure to store session forwarding rules in UPF
struct UpfSession {
    int ueId;               ///< Unique identifier for the UE
    int sessionId;          ///< Unique session identifier
    std::string destination; ///< Forwarding destination (e.g., Internet, local network)

    /// Constructor to initialize UPF session
    UpfSession(int id, int sessId, std::string dest)
        : ueId(id), sessionId(sessId), destination(dest) {}
};

/// Class to manage active sessions in UPF
class UpfSessionTable {
  private:
    std::unordered_map<int, UpfSession> sessionTable;  ///< Tracks active session flows

  public:
    /// Add a new session
    void addSession(int ueId, int sessionId, std::string destination) {
        int sessionKey = ueId * 1000 + sessionId; // Unique key for each session
        sessionTable[sessionKey] = UpfSession(ueId, sessionId, destination);
    }

    /// Check if a session exists
    bool hasSession(int ueId, int sessionId) {
        int sessionKey = ueId * 1000 + sessionId;
        return sessionTable.count(sessionKey) > 0;
    }

    /// Remove a session from UPF
    bool removeSession(int ueId, int sessionId) {
        int sessionKey = ueId * 1000 + sessionId;
        return sessionTable.erase(sessionKey);
    }

    /// Retrieve session details (for debugging/logging)
    UpfSession* getSession(int ueId, int sessionId) {
        int sessionKey = ueId * 1000 + sessionId;
        if (sessionTable.count(sessionKey))
            return &sessionTable[sessionKey];
        return nullptr;
    }
};


#endif /* CONTEXTS_UPFSESSIONTABLE_H_ */
