#include"JsonParser.h"

#include<iostream>
#include<cctype>

JsonParser::JsonParser(const std::string& input) {
	m_text = input;
	m_position = 0;
}

char JsonParser::current()
{
	if (m_position >= m_text.size())
		throw std::runtime_error("Unexpected end of JSON");

	return m_text[m_position];
}

void JsonParser::skipWhitespace() {
	while (m_position < m_text.size() && std::isspace(static_cast<unsigned char>(m_text[m_position]))) m_position++;
}

JsonValue JsonParser::parse() {
	skipWhitespace();
	return parseValue();
}

JsonValue JsonParser::parseValue()
{
	skipWhitespace();

	char c = current();

	std::cout << "Parsing character: [" << c << "]\n";


	if (c == '{')
		return parseObject();

	if (c == '[')
		return parseArray();

	if (c == '"')
		return parseString();

	if (c >= '0' && c <= '9')
		return parseNumber();


	if (m_text.compare(m_position, 4, "true") == 0)
	{
		m_position += 4;
		return JsonValue(true);
	}


	if (m_text.compare(m_position, 5, "false") == 0)
	{
		m_position += 5;
		return JsonValue(false);
	}


	if (m_text.compare(m_position, 4, "null") == 0)
	{
		m_position += 4;
		return JsonValue();
	}

	throw std::runtime_error(
		"Invalid JSON"
	);
}

JsonValue JsonParser::parseString() {
	m_position++;

	std::string result;

	while (m_position < m_text.size()) {
		char c = current();

		if (c == '"') {
			m_position++;
			break;
		}

		if (c == '\\') {
			m_position++;

			char escaped = current();

			switch (escaped) {
			case '"':
				result += '"';
				break;

			case '\\':
				result += '\\';
				break;

			case 'n':
				result += '\n';
				break;

			case 't':
				result += '\t';
				break;

			default:
				result += escaped;
			}


			m_position++;
		} else {
			result += c;
			m_position++;
		}
	}

	return JsonValue(result);
}

JsonValue JsonParser::parseNumber() {
	std::string number;

	while (m_position < m_text.size() &&
		((m_text[m_position] >= '0' && m_text[m_position] <= '9') ||
			m_text[m_position] == '.' ||
			m_text[m_position] == '-' ||
			m_text[m_position] == '+' ||
			m_text[m_position] == 'e' ||
			m_text[m_position] == 'E'))
	{
		number += m_text[m_position];
		m_position++;
	}

	return JsonValue(std::stod(number));
}

JsonValue JsonParser::parseArray() {

	JsonArray array;
	m_position++;

	skipWhitespace();

	while (current() != ']') {
		array.push_back(parseValue());
		skipWhitespace();

		if (current() == ',') m_position++;
		skipWhitespace();
	}


	m_position++;

	return JsonValue(array);
}

JsonValue JsonParser::parseObject() {
	JsonObject object;
	m_position++;

	skipWhitespace();

	while (current() != '}') {
		JsonValue key = parseString();
		skipWhitespace();

		if (current() != ':') {
			throw std::runtime_error("Expected :");
		}

		m_position++;

		skipWhitespace();

		JsonValue value = parseValue();
		object[key.asString()] = value;

		skipWhitespace();

		if (current() == ',') {
			m_position++;
			skipWhitespace();
		}
	}

	m_position++;

	return JsonValue(object);

}