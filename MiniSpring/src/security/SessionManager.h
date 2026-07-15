#pragma once

#include"Authentication.h"

#include<unordered_map>
#include<optional>
#include<mutex>
#include<random>
#include<sstream>
#include<iomanip>

class SessionManager {
public:
	std::string createSession(const Authentication& auth) {
		std::string id = generateSessionId();
		std::lock_guard<std::mutex> lock(m_mutex);
		m_sessions[id] = auth;
		return id;
	}

	std::optional<Authentication> getSession(const std::string& id) {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = m_sessions.find(id);
		if (it == m_sessions.end()) return std::nullopt;
		return it->second;
	}

	void destroySession(const std::string& id) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_sessions.erase(id);
	}

private:
	// std::random_device + mt19937_64, 128 bits of entropy hex-encoded --
	// session IDs must be unguessable, not just unique, or an attacker who
	// can predict the next ID can hijack another user's session outright.
	static std::string generateSessionId() {
		std::random_device rd;
		std::mt19937_64 gen1(rd()), gen2(rd());
		std::uniform_int_distribution<uint64_t> dist;

		std::stringstream ss;
		ss << std::hex << std::setfill('0') << std::setw(16) << dist(gen1)
			<< std::setw(16) << dist(gen2);
		return ss.str();
	}

private:
	std::unordered_map<std::string, Authentication> m_sessions;
	std::mutex m_mutex;
};