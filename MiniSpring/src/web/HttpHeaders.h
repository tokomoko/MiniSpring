#pragma once

#include"../core/Core.h"

#include <string>
#include <unordered_map>

class MINISPRING_API HttpHeaders {
public:
	void set(const std::string& name, const std::string& value);
	std::string get(const std::string& name) const;

	bool contains(const std::string& name) const;

	const std::unordered_map<std::string, std::string>& all() const;

private:
	std::unordered_map<std::string, std::string> m_headers;

};
