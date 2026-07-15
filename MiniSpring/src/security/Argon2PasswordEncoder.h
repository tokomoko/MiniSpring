#pragma once

#include"IPasswordEncoder.h"

#include<cstdint>
#include<string>

class Argon2PasswordEncoder : public IPasswordEncoder {
public:
	std::string encode(const std::string& raw) override;
	bool matches(const std::string& raw, const std::string& encoded) override;

private:
	static constexpr uint32_t kTimeCost = 3;
	static constexpr uint32_t kMemoryCostKiB = 65536;
	static constexpr uint32_t kParallelism = 1;
	static constexpr size_t kSaltLength = 16;
	static constexpr size_t kHashLength = 32;
};