#include "EmbindProxy/module.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

std::string Module::getEmbind() const {
	std::string out;
	for (auto const& function : m_functions) {
		out += fmt::format("\t{};\n\n", function.getEmbind());
	}

	for (auto const& cls : m_classes) {
		out += fmt::format("\t{};\n\n", cls.getEmbind());
	}

	for (auto const& e : m_enums) {
		out += fmt::format("\t{};\n\n", e.getEmbind());
	}

	for (auto const& attribute : m_attributes) {
		out += fmt::format("\t{}.{};\n\n", m_prefix, attribute.getEmbind());
	}

	// Define all the children
	for (auto const& [submoduleName, submoduleVariable] : m_submodules) {
		out += fmt::format(
		    "\tauto {submoduleVariable} = {variableName}.def_submodule(\"{submoduleName}\");\n",
		    fmt::arg("submoduleVariable", submoduleVariable),
		    fmt::arg("submoduleName", submoduleName),
		    fmt::arg("variableName", m_prefix));
	}

	return out;
}

Module::Module(std::string const& prefix)
    : m_prefix(prefix), m_submodules({}), m_functions(), m_enums(),
      m_attributes() {}

void Module::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Module::addClass(Class const& c) {
	m_classes.push_back(c);
}

void Module::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Module::addAttribute(Attribute const& a) {
	m_attributes.push_back(a);
}

std::string const& Module::getPrefix() const {
	return m_prefix;
}

void Module::addSubmodule(std::string const& name,
                          std::string const& variableName) {
	m_submodules.push_back({name, variableName});
}

}    // namespace EmbindProxy
