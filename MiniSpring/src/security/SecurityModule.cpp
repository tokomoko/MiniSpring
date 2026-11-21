#include "SecurityModule.h"

#include "../core/ServiceProvider.h"
#include "../web/FilterChain.h"
#include "../web/Router.h"

#include "Authentication.h"
#include "AuthorizationFilter.h"
#include "IPasswordEncoder.h"
#include "IUserDetailsService.h"
#include "SecurityHeadersFilter.h"
#include "SessionAuthenticationFilter.h"

#include <initializer_list>

void SecurityModule::build(ServiceProvider &services) {
  services.get<EntityRegistry>().registerEntity<Session>();

  auto &userDetailsService = services.get<IUserDetailsService>();
  auto &passwordEncoder = services.get<IPasswordEncoder>();
  auto &router = services.get<Router>();
  auto &entityManager = services.get<EntityManager>();

  m_sessions = std::make_unique<SessionManager>(entityManager);

  m_authManager = std::make_unique<AuthenticationManager>();
  m_authManager->addProvider(std::make_unique<DaoAuthenticationProvider>(
      userDetailsService, passwordEncoder));

  services.addRef<AuthenticationManager>(*m_authManager);
  services.addRef<SessionManager>(*m_sessions);
  services.addRef<SecurityConfiguration>(m_configuration);

  services.get<PreFilterChain>().add(
      std::make_unique<SessionAuthenticationFilter>(
          *m_sessions, m_configuration.cookieName()));
  services.get<PostFilterChain>().add(
      std::make_unique<SecurityHeadersFilter>(m_configuration));
}

std::shared_ptr<FilterChain> SecurityModule::protect(
    std::vector<std::string> roles) {
  auto chain = std::make_shared<FilterChain>();
  chain->add(std::make_unique<AuthorizationFilter>(roles));
  return chain;
}

std::shared_ptr<IFilter> SecurityModule::authorize(
    std::initializer_list<std::string> roles) {
  return std::make_shared<AuthorizationFilter>(std::vector<std::string>(roles));
}