#include "Builders/moduleBuilder.hpp"
#include "Builders/attributeBuilder.hpp"
#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "EmbindProxy/module.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include "Helpers/combine.hpp"
#include "Helpers/getOverloadedFunctions.hpp"
#include "Helpers/split.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <fmt/format.h>
#include <set>
#include <string>

namespace Builders {

// Return a unique prefix name that can be used in the generated code for this module
std::string getPrefix(std::string qualifiedName) {
	// MyNS::Math -> MyNs_Math
	// This is to avoid naming conflicts when defining functions/classes with the
	// same name in different namespaces

	// If qualifiedName is the root name (global namespace has no name)
	// This will return ""
	auto prefix =
	    fmt::format("{}", fmt::join(Helpers::split(qualifiedName, "::"), "_"));
	return prefix.empty() ? prefix : prefix + '_';
}

std::optional<EmbindProxy::Module>
buildModule(IR::Namespace const& ns,
            EmbindProxy::TypeInfo& typeInfo) {
	EmbindProxy::Module builtModule(ns.m_name, getPrefix(ns.m_representation));

	auto overloadedFunctions = Helpers::getOverloadedFunctions(ns.m_functions);
	for (auto const& function : ns.m_functions) {
		if (auto maybeF = Builders::buildFunction(function, typeInfo)) {
			auto f = maybeF.value();
			if (overloadedFunctions.find(function.m_representation) !=
			    overloadedFunctions.end()) {
				f.setAsOverloaded();
			}
			builtModule.addFunction(f);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& variable : ns.m_variables) {
		auto v =
		    Builders::buildAttribute(ns.m_representation, variable, typeInfo);
		builtModule.addAttribute(v);
	}

	for (auto const& cls : ns.m_structs) {
		if (auto maybeC = Builders::buildClass(cls, typeInfo)) {
			auto c = maybeC.value();
			builtModule.addClass(c);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& e : ns.m_enums) {
		builtModule.addEnum(Builders::buildEnum(e));
	}

	return builtModule;
}
}    // namespace Builders
