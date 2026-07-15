#pragma once

#include<string>

class IPasswordEncoder {
public:
	virtual ~IPasswordEncoder() = default;
	virtual std::string encode(const std::string& raw) = 0;
	virtual bool matches(const std::string& raw, const std::string& encoded) = 0;
};