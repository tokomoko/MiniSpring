#include "MarkdownRenderer.h"

#include <sstream>

std::string MarkdownRenderer::render(const std::string &markdown) {
  std::stringstream input(markdown);
  std::string line;
  std::string html;

  bool inList = false;
  bool inCode = false;

  while (std::getline(input, line)) {
    if (line.rfind("```", 0) == 0) {
      if (!inCode) {
        html += "<pre><code>";
        inCode = true;
      } else {
        html += "</code></pre>";
        inCode = false;
      }
      continue;
    }

    if (inCode) {
      html += escapeHtml(line);
      html += "\n";
      continue;
    }

    if (startsWith(line, "# ")) {
      html += "<h1>";
      html += renderInline(line.substr(2));
      html += "</h1>";
      continue;
    }

    if (startsWith(line, "## ")) {
      html += "<h2>";
      html += renderInline(line.substr(3));
      html += "</h2>";
      continue;
    }

    if (line.size() > 0 && (line[0] == '-' || line[0] == '*') &&
        line.size() > 1 && line[1] == ' ') {
      if (!inList) {
        html += "<ul>";
        inList = true;
      }

      html += "<li>";
      html += renderInline(line.substr(2));
      html += "</li>";
      continue;
    } else {
      if (inList) {
        html += "</ul>";
        inList = false;
      }
    }

    if (line.empty()) {
      continue;
    }

    html += "<p>";
    html += renderInline(line);
    html += "</p>";
  }

  if (inList) html += "</ul>";
  if (inCode) html += "</code></pre>";
  return html;
}

std::string MarkdownRenderer::renderInline(const std::string &text) {
  std::string result = escapeHtml(text);
  size_t pos;

  while ((pos = result.find("`")) != std::string::npos) {
    size_t end = result.find("`", pos + 1);
    if (end == std::string::npos) break;

    std::string code = result.substr(pos + 1, end - pos - 1);

    result.replace(pos, end - pos + 1, "<code>" + code + "</code>");
  }

  while ((pos = result.find("**")) != std::string::npos) {
    size_t end = result.find("**", pos + 2);
    if (end == std::string::npos) break;

    std::string bold = result.substr(pos + 2, end - pos - 2);
    result.replace(pos, end - pos + 2, "<strong>" + bold + "</strong>");
  }

  while ((pos = result.find("*")) != std::string::npos) {
    size_t end = result.find("*", pos + 1);
    if (end == std::string::npos) break;

    std::string italic = result.substr(pos + 1, end - pos - 1);
    result.replace(pos, end - pos + 1, "<em>" + italic + "</em>");
  }

  return result;
}

std::string MarkdownRenderer::escapeHtml(const std::string &text) {
  std::string result;

  for (char c : text) {
    switch (c) {
      case '&':
        result += "&amp;";
        break;
      case '<':
        result += "&lt;";
        break;
      case '>':
        result += "&gt;";
        break;
      case '"':
        result += "&quot;";
        break;
      default:
        result += c;
    }
  }

  return result;
}

bool MarkdownRenderer::startsWith(const std::string &text,
                                  const std::string &prefix) {
  return text.rfind(prefix, 0) == 0;
}