#include "EmbindProxy/module.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

std::string Module::getEmbind() const {
	std::string out;
	for (auto const& function : m_functions) {
		out += fmt::format("\t{};\n\n", function.getEmbind(m_namePrefix));
	}

	for (auto const& cls : m_classes) {
		out += fmt::format("\t{};\n\n", cls.getEmbind(m_namePrefix));
	}

	for (auto const& e : m_enums) {
		out += fmt::format("\t{};\n\n", e.getEmbind());
	}

	for (auto const& attribute : m_attributes) {
		out += fmt::format("\t{};\n\n", attribute.getEmbind());
	}

	return out;
}

namespace {
std::string getModuleDeclaration(std::string const& name, int level) {
	return level == 1 ? fmt::format("Module['{}'] =", name) :
                        fmt::format("{}:", name);
}
}    // namespace

void Module::setLevel(int level) {
	m_level = level;
}

std::string Module::getPreJS() const {
	if (m_name.empty()) {
		// The global namespace is not touched in the preJS
		return "";
	}

	// If isFirstLevel
	// Module['Stuff'] = {
	// else
	// Inner: {
	std::string out = fmt::format(
	    R"(
{moduleDeclaration} {{
}},
)",
	    fmt::arg("moduleDeclaration", getModuleDeclaration(m_name, m_level)));

	return out;
}

Module::Module(std::string const& name, std::string const& prefix)
    : m_name(name), m_namePrefix(prefix), m_functions(), m_enums(),
      m_attributes(), m_level(0) {}

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
	return m_namePrefix;
}

}    // namespace EmbindProxy
