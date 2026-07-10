#pragma once

#include <string>
#include "Model.h"

class View {
public:
	View(const std::string& name, const Model& model) : m_name(name), m_model(model) {}

	const std::string& name() const {
		return m_name;
	}

	const Model& model() const {
		return m_model;
	}

private:

	std::string m_name;
	Model m_model;
};