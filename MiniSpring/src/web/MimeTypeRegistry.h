#pragma once

#include<string>
#include<unordered_map>

class MimeTypeRegistry
{
public:
	MimeTypeRegistry();

	std::string get(const std::string& extension) const;
	void registerType(const std::string& extension, const std::string& mimeType);

private:
	std::unordered_map<std::string, std::string> m_types;
};