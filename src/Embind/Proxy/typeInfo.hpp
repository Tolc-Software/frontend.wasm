#pragma once

#include <set>
#include <string>

namespace Embind::Proxy {

struct TypeInfo {
	// E.g. Use of std::vector<std::string> requires
	//      register_vector<std::string>("vector_string")
	std::set<std::string> m_registerCommands;

	// If a class is managed by a shared_ptr, its fully qualified name will be here
	// A class will be marked shared if it is handled by a shared_ptr
	std::set<std::string> m_classesMarkedShared;

	// Extra functions that are needed before the actual call to EMSCRIPTEN_BINDINGS
	// For example:
	// Non const class member variables require getters and setters
	// to be available at compile time.
	// Since we cannot be sure any of the class functions are getters or setters
	// we need to define them ourselves.
	// E.g.:
	//
	//     decltype(MyClass::member) MyClass_get_member(MyClass const& c) {
	//       return c.member;
	//     }
	std::set<std::string> m_extraFunctions;

	// Do we need to include anything extra?
	// Example entries: {<string>, "MyStuff.hpp"}
	std::set<std::string> m_includes;

	// The namespace under which all extra defined functions are placed to avoid conflicts
	std::string m_functionsNamespace = "Tolc_";
};

}    // namespace Embind::Proxy
