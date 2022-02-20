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
	std::string out =
	    fmt::format("\tem::class_<{fullyQualifiedName}>(\"{name}\")\n",
	                fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	                fmt::arg("name", createName(namePrefix)));

	if (m_isManagedByShared) {
		out += fmt::format(
		    "\t\t.smart_ptr_constructor(\"{fullyQualifiedName}\", &std::make_shared<{fullyQualifiedName}>\n",
		    fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
	}

	for (auto const& init : m_constructors) {
		out += fmt::format("\t\t.{constructorEmbind}",
		                   fmt::arg("constructorEmbind", init.getEmbind()));
	}

	for (auto const& function : m_functions) {
		out += fmt::format("\t\t.{functionEmbind}",
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

	// Remove the last newline
	out.pop_back();

	// End the class statement and put in a few newlines before enums
	out += ";\n\n";

	// To put the enums at the end of the class
	// we have to do some trickery since we are not allowed to insert the last ';'
	if (!m_enums.empty()) {

		for (auto const& e : m_enums) {
			// Need to add class prefix to nested enums
			// i.e ClassName_Enum
			// or MyNamespace_ClassName_Enum
			out += e.getEmbind(getClassNamePrefix(namePrefix, m_name));
		}
	}

	return out;
}

std::string
Class::getGlobalPreJS(std::string const& namePrefix,
                      std::vector<std::string>& namesToDelete) const {
	// Expects to be injected where necessary
	std::string preJS;
	for (auto const& e : m_enums) {
		// Will no longer need the old name
		namesToDelete.push_back(
		    e.createName(getClassNamePrefix(namePrefix, m_name)));

		// Rename enums that are declared within the class
		// Since this class will then be moved to it's correct namespace
		// The renamed enum will follow
		preJS += fmt::format(
		    R"(
Module['{globalName}']['{enumName}'] = Module['{globalEnumName}'];
)",
		    fmt::arg("globalName", createName(namePrefix)),
		    fmt::arg("enumName", e.getName()),
		    fmt::arg("globalEnumName", namesToDelete.back()));
	}
	return preJS;
}

std::string Class::getPreJS(std::string const& namePrefix,
                            std::vector<std::string>& namesToDelete) const {
	// Will no longer need the old name
	namesToDelete.push_back(createName(namePrefix));

	// Renaming the class
	// Expects to be injected where necessary
	return fmt::format(R"(
{baseName}: Module['{globalName}'],
)",
	                   fmt::arg("baseName", m_name),
	                   fmt::arg("globalName", namesToDelete.back()));
}

std::string Class::createName(std::string const& namePrefix) const {
	return namePrefix + m_name;
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
