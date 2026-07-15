#pragma once

#include"../core/Core.h"

#include"../module/Module.h"
#include"../web/RouteMetadata.h"

#include"SecurityConfiguration.h"
#include"AuthenticationManager.h"
#include"SessionManager.h"
#include"DaoAuthenticationProvider.h"

class ServiceProvider;

class MINISPRING_API SecurityModule : public Module {
public:
	SecurityConfiguration& configuration() { return m_configuration; }
	AuthenticationManager& authenticationManager() { return *m_authManager; }
	RouteMetadata protect(std::vector<std::string> authorities = {}) {
		return RouteMetadata{ true, std::move(authorities) };
	}

	void build(ServiceProvider& services) override;

private:
	SecurityConfiguration m_configuration;
	std::unique_ptr<AuthenticationManager> m_authManager;
	std::unique_ptr<SessionManager> m_sessions;
};