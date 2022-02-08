#include "Builders/functionBuilder.hpp"
#include "EmbindProxy/function.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include "Helpers/Embind/checkType.hpp"
#include "Helpers/types.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace Builders {

std::optional<EmbindProxy::Function>
buildFunction(IR::Function const& cppFunction,
              EmbindProxy::TypeInfo& typeInfo) {
	EmbindProxy::Function jsFunction(
	    Helpers::removeCppTemplate(cppFunction.m_name),
	    cppFunction.m_representation);

	std::set<std::string> includes;

	for (auto const& arg : cppFunction.m_arguments) {
		Helpers::Embind::checkType(arg.m_type, typeInfo);

		jsFunction.addArgument(arg.m_type.m_representation, arg.m_name);
	}

	Helpers::Embind::checkType(cppFunction.m_returnType, typeInfo);
	jsFunction.setReturnType(cppFunction.m_returnType.m_representation);

	return jsFunction;
}
}    // namespace Builders