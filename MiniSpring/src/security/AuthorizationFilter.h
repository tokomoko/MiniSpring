#pragma once

#include"../web/Filter.h"
#include"../web/Router.h"
#include"../web/HttpRequest.h"
#include"../web/HttpResponse.h"
#include"../web/HttpError.h"

#include"SecurityContext.h"

#include<algorithm>

class AuthorizationFilter : public IFilter {
public:
	explicit AuthorizationFilter(Router& router) : m_router(router) {}

	bool doFilter(HttpRequest& request, HttpResponse& response) override {
		const Router::Route* route = m_router.findRoute(request.method(), request.path(), request);

		if (!route || !route->metadata.requiresAuth)
			return true;

		auto& auth = SecurityContextHolder::getContext().authentication();

		if (!auth.isAuthenticated()) {
			response.status(401);
			response.json(HttpError(401, "Authentication required").toJson());
			return false;
		}

		auto& required = route->metadata.requiredAuthorities;
		if (!required.empty()) {
			bool hasAny = std::any_of(auth.authorities().begin(), auth.authorities().end(),
				[&](const std::string& a) {
				return std::find(required.begin(), required.end(), a) != required.end();
			});

			if (!hasAny) {
				response.status(403);
				response.json(HttpError(403, "Insufficient permissions").toJson());
				return false;
			}
		}

		return true;
	}

private:
	Router& m_router;
};