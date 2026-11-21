#include "Argon2PasswordEncoder.h"

//#include<argon2.h>

#include <cstring>
#include <random>
#include <stdexcept>
#include <vector>

std::string Argon2PasswordEncoder::encode(const std::string &raw) {
  std::vector<uint8_t> salt(kSaltLength);

  //std::random_device rd;
  //for (auto &byte : salt) byte = static_cast<uint8_t>(rd());

  return "";
}

bool Argon2PasswordEncoder::matches(const std::string &raw,
                                    const std::string &encoded) {
  return false;  // argon2id_verify(encoded.c_str(), raw.data(), raw.size()) ==
                 // ARGON2_OK;
}