#include "MiniSpringApp.h"
#include "../orm/EntityRegistry.h"

MiniSpringApp::MiniSpringApp() {
  m_services.add<EntityRegistry>(std::make_unique<EntityRegistry>());
}

void MiniSpringApp::configure() { m_modules.configure(m_services); }

void MiniSpringApp::build() { m_modules.build(m_services); }

void MiniSpringApp::listen() { m_modules.start(m_services); }

void MiniSpringApp::stop() { m_modules.stop(m_services); }