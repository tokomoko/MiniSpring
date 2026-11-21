#include "PBKDF2PasswordEncoder.h"

#define NOMINMAX
#include <bcrypt.h>
#include <wincrypt.h>
#include <windows.h>

#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "crypt32.lib")

#include <cstring>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
std::vector<uint8_t> derive(const std::string &raw,
                            const std::vector<uint8_t> &salt,
                            uint32_t iterations, size_t keyLength) {
  BCRYPT_ALG_HANDLE hAlg = nullptr;

  if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, nullptr,
                                  BCRYPT_ALG_HANDLE_HMAC_FLAG) != 0)
    throw std::runtime_error("PBKDF2: failed to open HMAC-SHA256 provider");

  std::vector<uint8_t> derived(keyLength);

  NTSTATUS status = BCryptDeriveKeyPBKDF2(
      hAlg, (PUCHAR)raw.data(), (ULONG)raw.size(),
      (PUCHAR) const_cast<uint8_t *>(salt.data()), (ULONG)salt.size(),
      iterations, derived.data(), (ULONG)derived.size(), 0);

  BCryptCloseAlgorithmProvider(hAlg, 0);

  if (status != 0) throw std::runtime_error("PBKDF2: key derivation failed");
  return derived;
}

std::string base64Encode(const std::vector<uint8_t> &data) {
  DWORD outLen = 0;
  CryptBinaryToStringA(data.data(), (DWORD)data.size(),
                       CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr,
                       &outLen);
  std::string out(outLen, '\0');
  CryptBinaryToStringA(data.data(), (DWORD)data.size(),
                       CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, out.data(),
                       &outLen);
  out.resize(std::strlen(out.c_str()));
  return out;
}

std::vector<uint8_t> base64Decode(const std::string &text) {
  DWORD outLen = 0;
  CryptStringToBinaryA(text.c_str(), (DWORD)text.size(), CRYPT_STRING_BASE64,
                       nullptr, &outLen, nullptr, nullptr);
  std::vector<uint8_t> out(outLen);
  CryptStringToBinaryA(text.c_str(), (DWORD)text.size(), CRYPT_STRING_BASE64,
                       out.data(), &outLen, nullptr, nullptr);
  return out;
}

bool constantTimeEquals(const std::vector<uint8_t> &a,
                        const std::vector<uint8_t> &b) {
  if (a.size() != b.size()) return false;

  uint8_t diff = 0;
  for (size_t i = 0; i < a.size(); i++) diff |= a[i] ^ b[i];

  return diff == 0;
}
}  // namespace

std::string PBKDF2PasswordEncoder::encode(const std::string &raw) {
  std::vector<uint8_t> salt(kSaltLength);

  BCRYPT_ALG_HANDLE hRng = nullptr;
  if (BCryptOpenAlgorithmProvider(&hRng, BCRYPT_RNG_ALGORITHM, nullptr, 0) != 0)
    throw std::runtime_error("PBKDF2: failed to open RNG provider");

  NTSTATUS status = BCryptGenRandom(hRng, salt.data(), (ULONG)salt.size(), 0);
  BCryptCloseAlgorithmProvider(hRng, 0);

  if (status != 0) throw std::runtime_error("PBKDF2: salt generation failed");

  auto hash = derive(raw, salt, kIterations, kHashLength);

  std::stringstream out;
  out << "$pbkdf2-sha256$i=" << kIterations << "$" << base64Encode(salt) << "$"
      << base64Encode(hash);

  return out.str();
}

bool PBKDF2PasswordEncoder::matches(const std::string &raw,
                                    const std::string &encoded) {
  static const std::string prefix = "$pbkdf2-sha256$i=";
  if (encoded.rfind(prefix, 0) != 0) return false;

  size_t pos = prefix.size();
  size_t iterEnd = encoded.find('$', pos);
  if (iterEnd == std::string::npos) return false;

  uint32_t iterations = 0;
  try {
    iterations =
        static_cast<uint32_t>(std::stoul(encoded.substr(pos, iterEnd - pos)));
  } catch (...) {
    return false;
  }

  size_t saltStart = iterEnd + 1;
  size_t saltEnd = encoded.find('$', saltStart);
  if (saltEnd == std::string::npos) return false;

  auto salt = base64Decode(encoded.substr(saltStart, saltEnd - saltStart));
  auto storedHash = base64Decode(encoded.substr(saltEnd + 1));

  auto computed = derive(raw, salt, iterations, storedHash.size());
  return constantTimeEquals(computed, storedHash);
}