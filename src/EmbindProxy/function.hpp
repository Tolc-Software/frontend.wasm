#pragma once

#include <optional>
#include <string>
#include <vector>

namespace EmbindProxy {
class Function {
public:
	Function(std::string const& name, std::string const& fullyQualifiedName);

	/**
	* Creates a string corresponding to the pybind11 conversion of this function.
	* E.g.
	*   def("myFunction", &SomeNamespace::myFunction)
	*/
	std::string getEmbind() const;

	/**
	* Adds an argument name.
	* E.g.
	*   f(int i) would require addArgument("int", "i")
	*/
	void addArgument(std::string const& typeName, std::string const& name = "");

	void setAsClassFunction();

	void setAsConstructor();

	// Is there another function with the same name but with different arguments?
	void setAsOverloaded();

	void setAsStatic();

	// E.g. std::string f();
	// should call setReturnType("std::string")
	void setReturnType(std::string const& typeName);

private:
	std::string getSignature() const;
	std::string getArgumentTypes() const;

	// => {m_name}_{argType0}_{argType1}
	// If there are no argument types, then just {m_name}
	std::string createOverloadedName() const;

	struct Argument {
		// E.g.
		//   f(int i)
		// Would result in
		//   typeName = "int"
		//   name = "i"
		std::string typeName;
		std::string name;
	};

	// User defined name of the function
	std::string m_name;
	std::string m_fullyQualifiedName;
	// Defaults to void
	std::string m_returnType;
	std::vector<Argument> m_arguments;

	bool m_isConstructor;
	bool m_isOverloaded;
	bool m_isStatic;
	bool m_isClassFunction;
};
}    // namespace EmbindProxy
