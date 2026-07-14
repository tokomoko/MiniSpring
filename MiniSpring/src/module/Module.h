#pragma once

class ServiceProvider;

class Module {
public:
	virtual ~Module() = default;

	virtual void configure(ServiceProvider& services) {}
	virtual void build(ServiceProvider& services) {}
	virtual void start(ServiceProvider& services) {}
	virtual void stop(ServiceProvider& services) {}
};