#include"SecurityModule.h"

#include"../core/ServiceProvider.h"
#include"../web/FilterChain.h"
#include"../web/Router.h"

#include"SessionAuthenticationFilter.h"
#include"IUserDetailsService.h"
#include"IPasswordEncoder.h"
#include"AuthorizationFilter.h"

void SecurityModule::build(ServiceProvider& services) {
	services.get<EntityRegistry>().registerEntity<Session>();

	auto& userDetailsService = services.get<IUserDetailsService>();
	auto& passwordEncoder = services.get<IPasswordEncoder>();
	auto& router = services.get<Router>();
	auto& entityManager = services.get<EntityManager>();

	m_sessions = std::make_unique<SessionManager>(entityManager);

	m_authManager = std::make_unique<AuthenticationManager>();
	m_authManager->addProvider(std::make_unique<DaoAuthenticationProvider>(userDetailsService, passwordEncoder));

	services.addRef<AuthenticationManager>(*m_authManager);
	services.addRef<SessionManager>(*m_sessions);
	services.addRef<SecurityConfiguration>(m_configuration);

	auto& chain = services.get<FilterChain>();
	chain.add(std::make_unique<SessionAuthenticationFilter>(*m_sessions, m_configuration.cookieName()));
	chain.add(std::make_unique<AuthorizationFilter>(router));
}