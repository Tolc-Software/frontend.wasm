#include "Builders/moduleBuilder.hpp"
#include "Builders/attributeBuilder.hpp"
#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Embind/Proxy/module.hpp"
#include "Embind/Proxy/typeInfo.hpp"
#include "Helpers/combine.hpp"
#include "Helpers/getOverloadedFunctions.hpp"
#include "Helpers/split.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <fmt/format.h>
#include <set>
#include <string>

namespace Builders {

std::optional<Embind::Proxy::Module>
buildModule(IR::Namespace const& ns, Embind::Proxy::TypeInfo& typeInfo) {
	Embind::Proxy::Module builtModule(ns.m_name, ns.m_representation);

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
