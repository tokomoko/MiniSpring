#pragma once

#include<string>

class SecurityConfiguration {
public:
	SecurityConfiguration& cookieName(const std::string& name) {
		m_cookieName = name;
		return *this;
	}

	const std::string& cookieName() const {
		return m_cookieName;
	}

private:
	std::string m_cookieName = "SESSIONID";
};