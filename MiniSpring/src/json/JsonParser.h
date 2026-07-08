#pragma once

#include"JsonValue.h"

#include<string>

class JsonParser {
public:
	JsonParser(const std::string& input);
	JsonValue parse();

private:
	char current();
	void skipWhitespace();

	JsonValue parseValue();
	JsonValue parseObject();
	JsonValue parseArray();
	JsonValue parseString();
	JsonValue parseNumber();

private:
	std::string m_text;
	size_t m_position;
};