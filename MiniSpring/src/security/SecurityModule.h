#pragma once

#include"../core/Core.h"

#include"../module/Module.h"
#include"../web/FilterChain.h"

#include"SecurityConfiguration.h"
#include"AuthenticationManager.h"
#include"SessionManager.h"
#include"DaoAuthenticationProvider.h"

#include<initializer_list>

class ServiceProvider;

class MINISPRING_API SecurityModule : public Module {
public:
	SecurityConfiguration& configuration() { return m_configuration; }
	AuthenticationManager& authenticationManager() { return *m_authManager; }
	std::shared_ptr<FilterChain> protect(std::vector<std::string> roles = {});

	std::shared_ptr<IFilter> authorize(std::initializer_list<std::string> roles);

	void build(ServiceProvider& services) override;

private:
	SecurityConfiguration m_configuration;
	std::unique_ptr<AuthenticationManager> m_authManager;
	std::unique_ptr<SessionManager> m_sessions;
};