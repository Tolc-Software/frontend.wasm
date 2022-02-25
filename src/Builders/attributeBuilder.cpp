#include "Builders/attributeBuilder.hpp"
#include "Embind/Proxy/typeInfo.hpp"
#include "Embind/checkType.hpp"
#include "Helpers/types.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <string>

namespace Builders {

Embind::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                        IR::Variable const& v,
                                        Embind::Proxy::TypeInfo& typeInfo) {
	Embind::checkType(v.m_type, typeInfo);

	std::string callName = v.m_name;
	if (!parentNamespace.empty()) {
		callName = parentNamespace + "::" + callName;
	}
	if (Helpers::isBaseType(v.m_type, IR::BaseType::String)) {
		// Not possible
		// Warn, but the best we can do for now is empty string

		// Results in empty string
		// em::constant("my_string", std_string);

		spdlog::warn(
		    "Global variables of type std::string (regarding {}) can currently only be bound to an empty string in javascript. To get around this you can either use std::string_view or const char*. See this issue for more information: https://github.com/emscripten-core/emscripten/issues/16275.",
		    v.m_name);
	} else if (Helpers::isBaseType(v.m_type, IR::BaseType::StringView)) {
		// Need to wrap in val and call data
		callName = fmt::format("em::val({}.data())", callName);

		// Make sure val is available
		typeInfo.m_includes.insert("<emscripten/val.h>");
	} else if (Helpers::isBaseType(v.m_type, IR::BaseType::Char) &&
	           v.m_type.m_numPointers != 0) {
		// Note that other types of char ptrs (wchar_t, char16_t etc.)
		// are not supported out of the box. They result in runtime error when loaded.

		// Need to wrap in val
		callName = fmt::format("em::val({})", callName);

		// Make sure val is available
		typeInfo.m_includes.insert("<emscripten/val.h>");
	}

	return Embind::Proxy::Attribute(v.m_name, callName);
}
}    // namespace Builders
