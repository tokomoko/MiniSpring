#pragma once

#include"../core/Core.h"

class ServiceProvider;

class MINISPRING_API Module {
public:
	virtual ~Module() = default;

	virtual void configure(ServiceProvider& services) {}
	virtual void build(ServiceProvider& services) {}
	virtual void start(ServiceProvider& services) {}
	virtual void stop(ServiceProvider& services) {}
};