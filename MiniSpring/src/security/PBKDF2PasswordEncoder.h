#pragma once

#include"../core/Core.h"

#include"IPasswordEncoder.h"

#include<cstdint>
#include<string>

class MINISPRING_API PBKDF2PasswordEncoder : public IPasswordEncoder {
public:
	std::string encode(const std::string& raw) override;
	bool matches(const std::string& raw, const std::string& encoded) override;

private:
	static constexpr uint32_t kIterations = 210000;
	static constexpr size_t kSaltLength = 16;
	static constexpr size_t kHashLength = 32;
};