#pragma once

#include"../core/Core.h"

#include<string>

enum class FrameOptions {
	NONE,
	DENY,
	SAME_ORIGIN
};

inline std::string toString(FrameOptions option) {
	switch (option) {
	case FrameOptions::DENY: return "DENY";
	case FrameOptions::SAME_ORIGIN: return "SAMEORIGIN";
	}

	return "DENY";
}

enum class ReferrerPolicy {
	NONE,
	NO_REFERRER,
	NO_REFERRER_WHEN_DOWNGRADE,
	ORIGIN,
	ORIGIN_WHEN_CROSS_ORIGIN,
	SAME_ORIGIN,
	STRICT_ORIGIN,
	STRICT_ORIGIN_WHEN_CROSS_ORIGIN,
	UNSAFE_URL
};

inline std::string toString(ReferrerPolicy policy) {
	switch (policy) {
	case ReferrerPolicy::NO_REFERRER: return "no-referrer";
	case ReferrerPolicy::NO_REFERRER_WHEN_DOWNGRADE: return "no-referrer-when-downgrade";
	case ReferrerPolicy::ORIGIN: return "origin";
	case ReferrerPolicy::ORIGIN_WHEN_CROSS_ORIGIN: return "origin-when-cross-origin";
	case ReferrerPolicy::SAME_ORIGIN: return "same-origin";
	case ReferrerPolicy::STRICT_ORIGIN: return "strict-origin";
	case ReferrerPolicy::STRICT_ORIGIN_WHEN_CROSS_ORIGIN: return "strict-origin-when-cross-origin";
	case ReferrerPolicy::UNSAFE_URL: return "unsafe-url";
	}

	return "strict-origin-when-cross-origin";
}

class MINISPRING_API SecurityConfiguration {
public:
	SecurityConfiguration& cookieName(const std::string& name) {
		m_cookieName = name;
		return *this;
	}

	const std::string& cookieName() const {
		return m_cookieName;
	}

	SecurityConfiguration& enabled(bool enabled) {
		m_enabled = enabled;
		return *this;
	}

	SecurityConfiguration& frameOptions(FrameOptions options) {
		m_frameOptions = options;
		return *this;
	}

	SecurityConfiguration& contentTypeOptions(bool enabled) {
		m_contentTypeOptions = enabled;
		return *this;
	}

	SecurityConfiguration& referrerPolicy(ReferrerPolicy policy) {
		m_referrerPolicy = policy;
		return *this;
	}

	SecurityConfiguration& permissionsPolicy(const std::string& policy) {
		m_permissionsPolicy = policy;
		return *this;
	}
	
	SecurityConfiguration& contentSecurityPolicy(const std::string& policy) {
		m_csp = policy;
		return *this;
	}

	bool enabled() const {
		return m_enabled;
	}

	FrameOptions frameOptions() const {
		return m_frameOptions;
	}

	bool contentTypeOptions() const {
		return m_contentTypeOptions;
	}

	ReferrerPolicy referrerPolicy() const {
		return m_referrerPolicy;
	}

	const std::string& permissionsPolicy() const {
		return m_permissionsPolicy;
	}

	const std::string& contentSecurityPolicy() const {
		return m_csp;
	}

private:
	std::string m_cookieName = "SESSIONID";

	FrameOptions m_frameOptions = FrameOptions::NONE;
	ReferrerPolicy m_referrerPolicy = ReferrerPolicy::NONE;

	std::string m_permissionsPolicy = "camera=(), microphone=(), geolocation=()";
	std::string m_csp = "default-src 'self'; object-src 'none'; frame-ancestors 'none';";

	bool m_enabled = true;
	bool m_contentTypeOptions = true;
};