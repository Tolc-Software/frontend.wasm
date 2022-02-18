#include "EmbindProxy/function.hpp"
#include "Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

namespace {

std::string getEmbindSelect(std::string const& signature,
                            std::string const& functionName) {
	return fmt::format("select_overload<{}>(&{})", signature, functionName);
}

std::string getFunctionPrefix(bool isClassFunction, bool isStatic) {
	// Defined inside a class
	// => No need for namespaces
	if (isClassFunction) {
		// Static functions are prefixed "class_"
		return isStatic ? "class_" : "";
	}
	return "\tem::";
}

}    // namespace

std::string Function::getEmbind(std::string const& namePrefix) const {
	std::string f;
	if (m_isConstructor) {
		// Results in
		// constructor<std::string, int, double>)
		f = fmt::format("constructor<{}>()\n", getArgumentTypes());
	} else {
		// Results in
		// class_function("myFunction", select_overload<void()>(&MyNS::myFunction))
		f = fmt::format(
		    R"({functionPrefix}function("{name}", {qualifiedName}){ending})",
		    fmt::arg("functionPrefix",
		             getFunctionPrefix(m_isClassFunction, m_isStatic)),
		    fmt::arg("name", createName(namePrefix)),
		    fmt::arg("ending", m_isClassFunction ? "\n" : ";\n"),
		    fmt::arg("qualifiedName",
		             m_isOverloaded ?
		                 getEmbindSelect(getSignature(), m_fullyQualifiedName) :
                         '&' + m_fullyQualifiedName));
	}

	return f;
}

std::string Function::getPreJS(std::string const& namePrefix) const {
	// Renaming the function
	// Expects to be injected where necessary
	return fmt::format(R"(
{baseName}: Module['{globalName}'],
)",
	                   fmt::arg("baseName", m_name),
	                   fmt::arg("globalName", createName(namePrefix)));
}

std::string Function::createOverloadedName() const {
	std::vector<std::string> types;
	for (auto const& [argType, _argName] : m_arguments) {
		types.push_back(argType);
	}
	auto joinedArgs = fmt::format("{}", fmt::join(types, "_"));

	return joinedArgs.empty() ? m_name : m_name + '_' + joinedArgs;
}

Function::Function(std::string const& name,
                   std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName),
      m_returnType("void"), m_arguments({}), m_isConstructor(false),
      m_isOverloaded(false), m_isStatic(false), m_isClassFunction(false) {}

void Function::addArgument(std::string const& typeName,
                           std::string const& name) {
	m_arguments.push_back({typeName, name});
};

void Function::setReturnType(std::string const& typeName) {
	m_returnType = typeName;
}

void Function::setAsClassFunction() {
	m_isClassFunction = true;
};

void Function::setAsConstructor() {
	m_isConstructor = true;
};

void Function::setAsStatic() {
	m_isStatic = true;
};

void Function::setAsOverloaded() {
	m_isOverloaded = true;
};

std::string Function::getArgumentTypes() const {
	// Get the typenames of the arguments
	std::vector<std::string> typeNames;
	std::transform(m_arguments.begin(),
	               m_arguments.end(),
	               std::back_inserter(typeNames),
	               [](auto const& argument) { return argument.typeName; });
	return fmt::format("{}", fmt::join(typeNames, ", "));
}

std::string Function::getSignature() const {
	return fmt::format(
	    R"(({returnType}({namespace}*)({arguments})))",
	    fmt::arg("returnType", m_returnType),
	    fmt::arg("namespace",
	             Helpers::removeSubString(m_fullyQualifiedName, m_name)),
	    fmt::arg("arguments", getArgumentTypes()));
}

std::string Function::createName(std::string const& namePrefix) const {
	return fmt::format(
	    R"({namePrefix}{name})",
	    fmt::arg("namePrefix", namePrefix),
	    fmt::arg("name", m_isOverloaded ? createOverloadedName() : m_name));
}
}    // namespace EmbindProxy
