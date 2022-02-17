#include "EmbindProxy/class.hpp"
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

namespace {
std::string getClassNamePrefix(std::string const& namespacePrefix,
                               std::string const& className) {
	return namespacePrefix.empty() ? className + '_' :
                                     namespacePrefix + className + '_';
}
}    // namespace

std::string Class::getEmbind(std::string const& namePrefix) const {
	std::string out = fmt::format(
	    "em::class_<{fullyQualifiedName}>(\"{namePrefix}{name}\")\n",
	    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	    fmt::arg("namePrefix", namePrefix),
	    fmt::arg("name", m_name));

	if (m_isManagedByShared) {
		out += fmt::format(
		    "\t\t.smart_ptr_constructor(\"{fullyQualifiedName}\", &std::make_shared<{fullyQualifiedName}>\n",
		    fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
	}

	for (auto const& init : m_constructors) {
		out += fmt::format("\t\t.{constructorEmbind}\n",
		                   fmt::arg("constructorEmbind", init.getEmbind()));
	}

	for (auto const& function : m_functions) {
		out += fmt::format("\t\t.{functionEmbind}\n",
		                   fmt::arg("functionEmbind", function.getEmbind()));
	}

	for (auto const& variable : m_memberVariables) {
		// Create a setter for the variable if it isn't const
		std::string setter =
		    variable.m_setter ?
		        fmt::format(", &{}", variable.m_setter.value()) :
                "";

		std::string isStatic = variable.m_isStatic ? "class_" : "";

		out += fmt::format(
		    "\t\t.{isStatic}property(\"{variableName}\", &{getter}{setter})\n",
		    fmt::arg("isStatic", isStatic),
		    fmt::arg("getter", variable.m_getter),
		    fmt::arg("setter", setter),
		    fmt::arg("variableName", variable.m_name));
	}

	// To put the enums at the end of the class
	// we have to do some trickery since we are not allowed to insert the last ';'
	if (!m_enums.empty()) {
		// End the class statement and put in a few newlines before enums
		out += ";\n\n";

		for (auto const& e : m_enums) {
			// Need to add class prefix to nested enums
			// i.e ClassName_Enum
			// or MyNamespace_ClassName_Enum
			out += fmt::format(
			    "{};\n", e.getEmbind(getClassNamePrefix(namePrefix, m_name)));
		}
	}

	// Remove the trailing endlines
	if (!out.empty()) {
		while (out.back() == '\n' && !out.empty()) {
			out.pop_back();
		}
	}

	return out;
}

Class::Class(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_constructors(),
      m_functions(), m_memberVariables(), m_enums(),
      m_isManagedByShared(false) {}

void Class::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Class::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Class::addConstructor(Function const& constructor) {
	m_constructors.push_back(constructor);
}

void Class::addMemberVariable(std::string const& variableName,
                              std::string const& getter,
                              std::optional<std::string> setter,
                              bool isStatic) {
	m_memberVariables.push_back({variableName, getter, setter, isStatic});
}

std::string const& Class::getName() const {
	return m_name;
}

void Class::setAsManagedByShared() {
	m_isManagedByShared = true;
}
}    // namespace EmbindProxy
