// =====================================================
// session.h  —  Simple Session Manager
// =====================================================
// Uses a Hash Map (std::map) to store login sessions.
// token -> teacher_id mapping
// Like a "who is logged in" lookup table.
// DAA Concept: Hash Map
// =====================================================

#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

// Cookie name used in browser
const std::string COOKIE_NAME = "fs_session";

// Using Hash Map — token string maps to teacher_id
// This is our in-memory session store
std::map<std::string, int>         sessionToTeacher;  // token -> teacher_id
std::map<std::string, std::string> sessionToName;     // token -> teacher_name

// =====================================================
// generateToken()
// Creates a random 32-character hex string as session ID
// =====================================================
std::string generateToken() {
    srand((unsigned int)time(0) + rand());
    std::ostringstream ss;
    for (int i = 0; i < 8; i++) {
        ss << std::hex << std::setw(4) << std::setfill('0') << (rand() % 0xFFFF);
    }
    return ss.str();
}

// =====================================================
// createSession()
// Stores teacher login — returns the new token
// DAA: Hash Map insert O(log n)
// =====================================================
std::string createSession(int teacherId, const std::string& name) {
    std::string token = generateToken();
    // Using Hash Map — insert token -> teacher_id
    sessionToTeacher[token] = teacherId;
    sessionToName[token]    = name;
    return token;
}

// =====================================================
// getSessionTeacherId()
// Looks up teacher_id from token
// DAA: Hash Map lookup O(log n)
// =====================================================
int getSessionTeacherId(const std::string& token) {
    // Using Hash Map — lookup token
    std::map<std::string,int>::iterator it = sessionToTeacher.find(token);
    if (it != sessionToTeacher.end()) return it->second;
    return -1;  // not found = not logged in
}

// =====================================================
// getSessionName()
// Returns teacher name from token
// =====================================================
std::string getSessionName(const std::string& token) {
    std::map<std::string,std::string>::iterator it = sessionToName.find(token);
    if (it != sessionToName.end()) return it->second;
    return "";
}

// =====================================================
// destroySession()
// Remove session (logout)
// DAA: Hash Map delete O(log n)
// =====================================================
void destroySession(const std::string& token) {
    sessionToTeacher.erase(token);
    sessionToName.erase(token);
}

// =====================================================
// isLoggedIn()
// Quick check if a token is valid
// =====================================================
bool isLoggedIn(const std::string& token) {
    return sessionToTeacher.find(token) != sessionToTeacher.end();
}

#endif // SESSION_H
