#pragma once

#include"../core/Core.h"

#include"../json/JsonParser.h"
#include"../json/JsonMapper.h"

#include"HttpHeaders.h"

#include<string>
#include<unordered_map>

class MINISPRING_API HttpRequest {
public:
	HttpRequest(const std::string& raw, JsonMapper& mapper);

	std::string method();
	std::string path();

	std::string cookie(const std::string& name) const;

	void setPathParameter(const std::string& name, const std::string& value);
	std::string pathParameter(const std::string& name) const;
	const std::string& queryString() const { return m_queryString; }

	std::string header(const std::string& name) {
		return m_headers.get(name);
	}

	const std::string& body() const {
		return m_body;
	}

	template<typename T>
	T json() {
		JsonParser parser(m_body);
		JsonValue value = parser.parse();
		return m_mapper.fromJson<T>(value);
	}

	JsonValue jsonValue() {
		JsonParser parser(m_body);
		return parser.parse();
	}

private:
	std::string m_requestMethod;
	std::string m_requestPath;
	std::string m_body;
	std::string m_queryString;

	std::unordered_map<std::string, std::string > m_pathParameters;

	JsonMapper& m_mapper;
	HttpHeaders m_headers;
};