#pragma once

#include"AppUserDetails.h"
#include"../models/User.h"
#include<MiniSpring.h>

class AppUserDetailsService : public IUserDetailsService {
public:
	explicit AppUserDetailsService(ServiceProvider& services) : m_services(services) {}

	std::optional<std::shared_ptr<UserDetails>> loadUserByUsername(const std::string& username) override {
		auto& manager = m_services.get<EntityManager>(); // resolved lazily -- see note above
		auto user = manager.findBy<User>("username", Value(username));
		if (!user) return std::nullopt;

		return std::shared_ptr<UserDetails>(std::make_shared<AppUserDetails>(*user));
	}

private:
	ServiceProvider& m_services;
};