#pragma once

#include"IAuthenticationProvider.h"

#include<memory>
#include<vector>

class AuthenticationManager {
public:
	void addProvider(std::unique_ptr<IAuthenticationProvider> provider) {
		m_providers.push_back(std::move(provider));
	}

	std::optional<Authentication> authenticate(const std::string& principal, const std::string& credentials) {
		for (auto& provider : m_providers) {
			auto result = provider->authenticate(principal, credentials);
			if (result) return result;
		}
		return std::nullopt;
	}

private:
	std::vector<std::unique_ptr<IAuthenticationProvider>> m_providers;
};