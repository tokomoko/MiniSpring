#include"SecurityModule.h"

#include"../core/ServiceProvider.h"
#include"../web/FilterChain.h"

#include"SessionAuthenticationFilter.h"
#include"IUserDetailsService.h"
#include"IPasswordEncoder.h"

void SecurityModule::build(ServiceProvider& services) {
	auto& userDetailsService = services.get<IUserDetailsService>();
	auto& passwordEncoder = services.get<IPasswordEncoder>();

	m_authManager = std::make_unique<AuthenticationManager>();
	m_authManager->addProvider(std::make_unique<DaoAuthenticationProvider>(userDetailsService, passwordEncoder));

	services.addRef<AuthenticationManager>(*m_authManager);
	services.addRef<SessionManager>(m_sessions);

	services.get<FilterChain>().add(std::make_unique<SessionAuthenticationFilter>(m_sessions, m_configuration.cookieName()));
}