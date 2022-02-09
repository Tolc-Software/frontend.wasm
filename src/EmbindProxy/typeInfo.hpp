#pragma once

#include <set>
#include <string>

namespace EmbindProxy {

struct TypeInfo {
	// E.g. Use of std::vector<std::string> requires
	//      register_vector<std::string>("vector_string")
	std::set<std::string> m_registerCommands;

	// If a class is managed by a shared_ptr, its fully qualified name will be here
	// A class will be marked shared if it is handled by a shared_ptr
	std::set<std::string> m_classesMarkedShared;
};

}    // namespace EmbindProxy
