#pragma once

#include"../web/Filter.h"
#include"../web/Router.h"
#include"../web/HttpRequest.h"
#include"../web/HttpResponse.h"
#include"../web/HttpError.h"

#include"SecurityContext.h"

#include<algorithm>
#include<functional>

class AuthorizationFilter : public IFilter {
public:
	AuthorizationFilter(std::vector<std::string> authorities) : m_authorities(std::move(authorities)) {}

	bool doFilter(HttpRequest& request, HttpResponse& response) override {
		auto& auth = SecurityContextHolder::getContext().authentication();

		if (!auth.isAuthenticated()) {
			response.status(401);
			response.json(HttpError(401, "Authentication required").toJson());
			return false;
		}

		bool allowed = m_authorities.empty() || std::any_of(auth.authorities().begin(), auth.authorities().end(), [&](const auto& a) {
			return std::find(m_authorities.begin(), m_authorities.end(), a) != m_authorities.end();
		});

		if (!allowed) {
			response.status(403);
			response.json(HttpError(403, "Insufficient permissions").toJson());
			return false;
		}

		return true;
	}

private:
	std::vector<std::string> m_authorities;
};