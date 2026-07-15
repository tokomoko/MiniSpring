#pragma once

#include<memory>
#include<optional>
#include<string>

#include"UserDetails.h"

class IUserDetailsService {
public:
	virtual ~IUserDetailsService() = default;
	virtual std::optional<std::shared_ptr<UserDetails>> loadUserByUsername(const std::string& username) = 0;
};