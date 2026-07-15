#pragma once

#include"../core/Core.h"
#include"Authentication.h"

class MINISPRING_API SecurityContext {
public:
	Authentication& authentication() { return m_authentication; }
	void setAuthentication(Authentication auth) { m_authentication = std::move(auth); }

private:
	Authentication m_authentication;
};

class MINISPRING_API SecurityContextHolder {
public:
	static SecurityContext& getContext() {
		thread_local SecurityContext context;
		return context;
	}

	// MUST be called at the start of handling every request (see
	// SessionAuthenticationFilter). worker threads are long-lived and
	// reused across many requests, so without this, request N+1 on the
	// same worker thread would silently inherit request N's identity.
	static void clearContext() {
		getContext() = SecurityContext();
	}
};