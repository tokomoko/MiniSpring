#include "TemplateEngine.h"

#include <fstream>
#include <sstream>

TemplateEngine::TemplateEngine(const std::filesystem::path& directory) : m_directory(directory) {}

std::string TemplateEngine::render(const View& view) {
	std::ifstream file(m_directory / view.name());
	if (!file) throw std::runtime_error("Template not found");

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string html = buffer.str();

	return replaceText(html, view.model());
}

std::string TemplateEngine::replaceText(std::string html, const Model& model) {
	size_t position = 0;
	while (true) {
		size_t attr = html.find("ms:text=", position);

		if (attr == std::string::npos)
			break;


		size_t quoteStart = html.find('"', attr);
		size_t quoteEnd = html.find('"', quoteStart + 1);

		std::string key = html.substr(quoteStart + 1, quoteEnd - quoteStart - 1);

		size_t tagStart = html.rfind('<', attr);
		size_t tagEnd = html.find('>', attr);

		std::string tagName = html.substr(tagStart + 1, html.find(' ', tagStart) - tagStart - 1);
		std::string closing = "</" + tagName + ">";

		size_t closingPos = html.find(closing, tagEnd);

		if (closingPos == std::string::npos)
			break;

		std::string value;
		if (model.contains(key))
			value = model.get(key).stringify();

		size_t attributeLength = quoteEnd - attr + 1;

		html.erase(attr, attributeLength);
		tagEnd -= attributeLength;

		html.insert(tagEnd + 1, value);
		position = tagEnd + value.size();
	}


	return html;
}