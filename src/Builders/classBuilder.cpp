#include "Builders/classBuilder.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeToStringBuilder.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include "Helpers/Embind/checkType.hpp"
#include "Helpers/combine.hpp"
#include "Helpers/getOverloadedFunctions.hpp"
#include "Helpers/types.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Builders {

namespace {

}    // namespace

std::optional<EmbindProxy::Class> buildClass(IR::Struct const& cppClass,
                                             EmbindProxy::TypeInfo& typeInfo) {
	EmbindProxy::Class pyClass(
	    Helpers::removeCppTemplate(cppClass.m_name) +
	        Builders::getTemplateParameterString(cppClass.m_templateArguments),
	    cppClass.m_representation);

	auto overloadedFunctions =
	    Helpers::getOverloadedFunctions(cppClass.m_public.m_functions);
	// Ignore private functions
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybePyFunction = buildFunction(function, typeInfo)) {
			auto jsFunction = maybePyFunction.value();

			if (function.m_isStatic) {
				jsFunction.setAsStatic();
			}

			if (overloadedFunctions.find(function.m_representation) !=
			    overloadedFunctions.end()) {
				jsFunction.setAsOverloaded();
			}

			pyClass.addFunction(jsFunction);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybePyFunction = buildFunction(constructor, typeInfo)) {
			auto jsFunction = maybePyFunction.value();

			if (constructor.m_isStatic) {
				jsFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				jsFunction.setAsOverloaded();
			}

			jsFunction.setAsConstructor();
			pyClass.addConstructor(jsFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		Helpers::Embind::checkType(variable.m_type, typeInfo);
		pyClass.addMemberVariable(variable.m_name,
		                          variable.m_type.m_isConst,
		                          variable.m_type.m_isStatic);
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor =
		    EmbindProxy::Function(pyClass.getName(), pyClass.getName());
		constructor.setAsConstructor();
		pyClass.addConstructor(constructor);
	}

	for (auto const& e : cppClass.m_public.m_enums) {
		pyClass.addEnum(buildEnum(e));
	}

	if (typeInfo.m_classesMarkedShared.contains(cppClass.m_representation)) {
		pyClass.setAsManagedByShared();
	}

	return pyClass;
}
}    // namespace Builders
