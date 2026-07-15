#include"Argon2PasswordEncoder.h"

//#include<argon2.h>

#include<cstring>
#include<random>
#include<stdexcept>
#include<vector>

std::string Argon2PasswordEncoder::encode(const std::string& raw) {
	std::vector<uint8_t> salt(kSaltLength);

	// random_device is backed by the OS CSPRNG on Windows (CryptGenRandom)
	// and Linux (getrandom/urandom) in practice -- filling salt bytes
	// directly from it rather than through a non-cryptographic PRNG.
	std::random_device rd;
	for (auto& byte : salt)
		byte = static_cast<uint8_t>(rd());

	//size_t encodedLength = argon2_encodedlen(kTimeCost, kMemoryCostKiB, kParallelism, (uint32_t)kSaltLength, (uint32_t)kHashLength, Argon2_id);
	//std::string encoded(encodedLength, '\0');

	//int result = argon2id_hash_encoded(kTimeCost, kMemoryCostKiB, kParallelism, raw.data(), raw.size(), salt.data(), salt.size(), kHashLength, encoded.data(), encoded.size());

	//if (result != ARGON2_OK) throw std::runtime_error(std::string("Argon2 hashing failed: ") + argon2_error_message(result));

	//encoded.resize(std::strlen(encoded.c_str())); // trim trailing NUL padding
	//return encoded;
	return "";
}

bool Argon2PasswordEncoder::matches(const std::string& raw, const std::string& encoded) {
	return false;// argon2id_verify(encoded.c_str(), raw.data(), raw.size()) == ARGON2_OK;
}