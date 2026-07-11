#pragma once

#include"../core/Core.h"

#include <string>

class MINISPRING_API MarkdownRenderer {
public:
	std::string render(const std::string& markdown);

private:
	std::string renderInline(const std::string& text);
	std::string escapeHtml(const std::string& text);

	bool startsWith(const std::string& text, const std::string& prefix);
};