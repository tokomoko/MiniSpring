#include"ModuleRegistry.h"

void ModuleRegistry::configure(ServiceProvider& services) {
	for (auto& module : m_modules)
		module->configure(services);
}

void ModuleRegistry::build(ServiceProvider& services) {
	for (auto& module : m_modules)
		module->build(services);
}

void ModuleRegistry::start(ServiceProvider& services) {
	for (auto& module : m_modules)
		module->start(services);
}

void ModuleRegistry::stop(ServiceProvider& services) {
	for (auto& module : m_modules)
		module->stop(services);
}