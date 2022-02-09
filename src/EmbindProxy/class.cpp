#include "EmbindProxy/class.hpp"
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

std::string Class::getEmbind() const {
	std::string out =
	    fmt::format("class_<{fullyQualifiedName}>(\"{name}\")\n",
	                fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
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
	// Remove the last newline
	out.pop_back();

	for (auto const& variable : m_memberVariables) {
		// Create a setter for the variable if it isn't const
		std::string setter =
		    variable.m_isConst ?
		        "" :
                fmt::format(
		            ", [](auto& _tolc_c, auto const& _tolc_v) {{ _tolc_c.{variableName} = _tolc_v; }}",
		            fmt::arg("variableName", variable.m_name));

		out += fmt::format(
		    "\t\t.property(\"{variableName}\", [](auto& _tolc_c) {{ return _tolc_c.{variableName}; }}{setter})\n",
		    fmt::arg("setter", setter),
		    fmt::arg("variableName", variable.m_name));
	}

	// To put the enums at the end of the class
	// we have to do some trickery since we are not allowed to insert the last ';'
	if (!m_enums.empty()) {
		// End the class statement and put in a few newlines before enums
		out += ";\n\n";

		for (auto const& e : m_enums) {
			out += fmt::format("{};\n", e.getEmbind(m_name));
		}
		// The last endline
		out.pop_back();
		// The last ';'
		out.pop_back();
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
                              bool isConst,
                              bool isStatic) {
	m_memberVariables.push_back({variableName, isConst, isStatic});
}

std::string const& Class::getName() const {
	return m_name;
}

void Class::setAsManagedByShared() {
	m_isManagedByShared = true;
}
}    // namespace EmbindProxy
