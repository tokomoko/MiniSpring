#pragma once

#include"../module/Module.h"

#include"SecurityConfiguration.h"
#include"AuthenticationManager.h"
#include"SessionManager.h"
#include"DaoAuthenticationProvider.h"

class ServiceProvider;

class SecurityModule : public Module {
public:
	SecurityConfiguration& configuration() { return m_configuration; }
	AuthenticationManager& authenticationManager() { return *m_authManager; }
	SessionManager& sessions() { return m_sessions; }

	void build(ServiceProvider& services) override;

private:
	SecurityConfiguration m_configuration;
	SessionManager m_sessions;
	std::unique_ptr<AuthenticationManager> m_authManager;
};