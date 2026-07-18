#pragma once

#include"../web/Filter.h"
#include"../web/HttpRequest.h"
#include"../web/HttpResponse.h"

#include"SecurityConfiguration.h"

class SecurityHeadersFilter : public IFilter {
public:
	SecurityHeadersFilter(const SecurityConfiguration& config) : m_config(config) {}

	bool doFilter(HttpRequest& request, HttpResponse& response) override {
		if (!m_config.enabled())
			return true;

		if (m_config.frameOptions() != FrameOptions::NONE) {
			response.header("X-Frame-Options", toString(m_config.frameOptions()));
		}

		if (m_config.referrerPolicy() != ReferrerPolicy::NONE) {
			response.header("Referrer-Policy", toString(m_config.referrerPolicy()));
		}

		if (m_config.contentTypeOptions()) {
			response.header("X-Content-Type-Options", "nosniff");
		}

		if (!m_config.permissionsPolicy().empty()) { 
			response.header("Permissions-Policy", m_config.permissionsPolicy());
		}

		if (!m_config.contentSecurityPolicy().empty()) {
			response.header("Content-Security-Policy", m_config.contentSecurityPolicy());
		}

		return true;
	}

private:
	const SecurityConfiguration& m_config;
};