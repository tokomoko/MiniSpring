#pragma once

#include"Model.h"
#include"View.h"

#include<string>
#include<filesystem>

class TemplateEngine {
public:
	TemplateEngine(const std::filesystem::path& directory);

	std::string render(const View& view);

private:
	std::string replaceText(std::string html, const Model& model);
	std::string replaceIf(std::string html, const Model& model);
	std::string replaceFor(std::string html, const Model& model);
	std::string replaceHref(std::string html, const Model& model);
	std::string resolveValue(const std::string& expression, const Model& model);

private:
	std::filesystem::path m_directory;
};