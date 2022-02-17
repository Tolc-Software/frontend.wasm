#pragma once

#include "EmbindProxy/attribute.hpp"
#include "EmbindProxy/class.hpp"
#include "EmbindProxy/enum.hpp"
#include "EmbindProxy/function.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include <string>
#include <vector>

namespace EmbindProxy {

struct Module {
	explicit Module(std::string const& name, std::string const& prefix);

	void addFunction(Function const& function);

	void addClass(Class const& c);

	void addEnum(Enum const& e);

	void addAttribute(Attribute const& a);

	std::string const& getPrefix() const;

	std::string getEmbind() const;

	void setLevel(int level);

	std::string getPreJS() const;

private:
	// The name of the associated namespace
	std::string m_name;

	// Every variable starts with this.
	// Should be "" for global modules
	// Should be "Nested_Namespace" for other modules
	// Will result in function "MyNS::func" being used from js as "MyNS_func"
	std::string m_namePrefix;

	std::vector<Function> m_functions;
	std::vector<Class> m_classes;
	std::vector<Enum> m_enums;
	std::vector<Attribute> m_attributes;

	// m_level is the amount of nested namespaces this corresponds to
	// i.e.
	//    // Global namespace here => m_level = 0
	//    namespace ThisModule {
	//    // One nested level => m_level = 1
	//    }
	int m_level;
};
}    // namespace EmbindProxy
