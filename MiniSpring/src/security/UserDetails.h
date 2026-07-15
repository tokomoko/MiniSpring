#pragma once

#include<string>
#include<vector>

class UserDetails {
public:
	virtual ~UserDetails() = default;

	virtual const std::string& username() const = 0;
	virtual const std::string& passwordHash() const = 0;
	virtual std::vector<std::string> authorities() const = 0;
	virtual bool isEnabled() const { return true; }
};