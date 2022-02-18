#include "EmbindProxy/module.hpp"
#include "Helpers/Embind/joinCalls.hpp"
#include "Helpers/split.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <numeric>
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
		out += fmt::format("\t{};\n\n", e.getEmbind(m_namePrefix));
	}

	for (auto const& attribute : m_attributes) {
		out += fmt::format("\t{};\n\n", attribute.getEmbind(m_namePrefix));
	}

	return out;
}

namespace {
// Return a unique prefix name that can be used in the generated code for this module
std::string createPrefix(std::deque<std::string> path) {
	// MyNS::Math -> MyNs_Math
	// This is to avoid naming conflicts when defining functions/classes with the
	// same name in different namespaces

	// If qualifiedName is the root name (global namespace has no name)
	// This will return ""
	auto prefix = fmt::format("{}", fmt::join(path, "_"));
	return prefix.empty() ? prefix : prefix + '_';
}

std::string getModuleDeclaration(std::deque<std::string> const& path) {
	std::string decl = "Module";
	for (auto const& p : path) {
		decl += fmt::format("['{}']", p);
	}

	return decl += " =";
}

}    // namespace

std::string Module::getPreJS() const {
	// Move things into classes that are not supported in Embind
	std::string preJS =
	    Helpers::Embind::joinGlobalPreJS(m_namePrefix, m_classes);

	if (m_path.empty()) {
		// The rest are renaming things into their namespace
		// For the global there are no renames
		return preJS;
	}

	using namespace Helpers::Embind;
	preJS += fmt::format(
	    R"(
{moduleDeclaration} {{
{functions}
{enums}
{attributes}
{classes}
}};
)",
	    fmt::arg("moduleDeclaration", getModuleDeclaration(m_path)),
	    fmt::arg("functions", joinPreJS(m_namePrefix, m_functions)),
	    fmt::arg("enums", joinPreJS(m_namePrefix, m_enums)),
	    fmt::arg("attributes", joinPreJS(m_namePrefix, m_attributes)),
	    fmt::arg("classes", joinPreJS(m_namePrefix, m_classes)));

	return preJS;
}

Module::Module(std::string const& name, std::string const& qualifiedName)
    : m_name(name), m_path(Helpers::split(qualifiedName, "::")), m_namePrefix(),
      m_functions(), m_enums(), m_attributes() {
	m_namePrefix = createPrefix(m_path);
}

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
