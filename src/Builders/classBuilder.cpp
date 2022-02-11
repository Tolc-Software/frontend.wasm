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
#include <fmt/format.h>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Builders {

std::optional<EmbindProxy::Class> buildClass(IR::Struct const& cppClass,
                                             EmbindProxy::TypeInfo& typeInfo) {
	EmbindProxy::Class jsClass(
	    Helpers::removeCppTemplate(cppClass.m_name) +
	        Builders::getSeparatedTypeString(cppClass.m_templateArguments, "_"),
	    cppClass.m_representation);

	auto overloadedFunctions =
	    Helpers::getOverloadedFunctions(cppClass.m_public.m_functions);
	// Ignore private functions
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybeJsFunction = buildFunction(function, typeInfo)) {
			auto jsFunction = maybeJsFunction.value();

			jsFunction.setAsClassFunction();

			if (function.m_isStatic) {
				jsFunction.setAsStatic();
			}

			if (overloadedFunctions.find(function.m_representation) !=
			    overloadedFunctions.end()) {
				jsFunction.setAsOverloaded();
			}

			jsClass.addFunction(jsFunction);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybeJsFunction = buildFunction(constructor, typeInfo)) {
			auto jsFunction = maybeJsFunction.value();

			jsFunction.setAsClassFunction();

			if (constructor.m_isStatic) {
				jsFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				jsFunction.setAsOverloaded();
			}

			jsFunction.setAsConstructor();
			jsClass.addConstructor(jsFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		Helpers::Embind::checkType(variable.m_type, typeInfo);
		std::string getter = "";
		std::optional<std::string> setter = std::nullopt;
		if (variable.m_type.m_isConst) {
			// If it is const, we can just give a reference to it
			getter = fmt::format(
			    "{fullyQualifiedClassName}::{variable}",
			    fmt::arg("fullyQualifiedClassName", cppClass.m_representation),
			    fmt::arg("variable", variable.m_name));
		} else {
			// Not const => Need to create setters and getters
			getter = fmt::format(R"({className}_get_{variable})",
			                     fmt::arg("variable", variable.m_name),
			                     fmt::arg("className", jsClass.getName()));
			setter = fmt::format(R"({className}_set_{variable})",
			                     fmt::arg("variable", variable.m_name),
			                     fmt::arg("className", jsClass.getName()));

			std::string getterFunction = fmt::format(
			    R"(
decltype({fullyQualifiedClassName}::{variable}) {getter}({fullyQualifiedClassName} const& c_) {{
	return c_.{variable};
}})",
			    fmt::arg("fullyQualifiedClassName", cppClass.m_representation),
			    fmt::arg("variable", variable.m_name),
			    fmt::arg("getter", getter));

			std::string setterFunction = fmt::format(
			    R"(
void {setter}({fullyQualifiedClassName}& c_, decltype({fullyQualifiedClassName}::{variable}) const& v_) {{
	c_.{variable} = v_;
}}
)",
			    fmt::arg("fullyQualifiedClassName", cppClass.m_representation),
			    fmt::arg("variable", variable.m_name),
			    fmt::arg("setter", setter.value()));

			// The getters may be in a namespace
			getter = typeInfo.m_functionsNamespace + "::" + getter;
			setter = typeInfo.m_functionsNamespace + "::" + setter.value();

			// Save them add to module later
			typeInfo.m_extraFunctions.insert(getterFunction);
			typeInfo.m_extraFunctions.insert(setterFunction);
		}
		jsClass.addMemberVariable(
		    variable.m_name, getter, setter, variable.m_type.m_isStatic);
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor =
		    EmbindProxy::Function(jsClass.getName(), jsClass.getName());
		constructor.setAsConstructor();
		jsClass.addConstructor(constructor);
	}

	for (auto const& e : cppClass.m_public.m_enums) {
		jsClass.addEnum(buildEnum(e));
	}

	if (typeInfo.m_classesMarkedShared.contains(cppClass.m_representation)) {
		jsClass.setAsManagedByShared();
	}

	return jsClass;
}
}    // namespace Builders
