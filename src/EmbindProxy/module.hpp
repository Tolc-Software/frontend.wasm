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
	explicit Module(std::string const& variableName);

	void addFunction(Function const& function);

	void addClass(Class const& c);

	void addEnum(Enum const& e);

	void addAttribute(Attribute const& a);

	std::string const& getPrefix() const;

	void addSubmodule(std::string const& name, std::string const& variableName);

	std::string getEmbind() const;

private:
	// Every variable starts with this.
	// Should be "" for global modules
	// Should be "Nested_Namespace" for other modules
	// Will result in function "MyNS::func" being used from js as "MyNS_func"
	std::string m_prefix;

	// If this is non empty,
	// then this module is a submodule
	// {Simple name, Variable name}
	// E.g.
	// {"Sub", "NS_Nested_Sub"}
	std::vector<std::pair<std::string, std::string>> m_submodules;

	std::vector<Function> m_functions;
	std::vector<Class> m_classes;
	std::vector<Enum> m_enums;
	std::vector<Attribute> m_attributes;
};
}    // namespace EmbindProxy
