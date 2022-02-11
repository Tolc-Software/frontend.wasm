#include "Helpers/Embind/checkType.hpp"
#include "Builders/typeToStringBuilder.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include "Helpers/types.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <queue>
#include <spdlog/spdlog.h>
#include <string>

namespace Helpers::Embind {

std::optional<std::string> getRegisterString(IR::ContainerType container) {
	// See https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
	switch (container) {
		case IR::ContainerType::Array:
			return R"(em::value_array<std::array<{}>>("{}"))";
			break;
		case IR::ContainerType::Map:
			return R"(em::register_map<{}>("{}"))";
			break;
		case IR::ContainerType::Vector:
			return R"(em::register_vector<{}>("{}"))";
			break;

		case IR::ContainerType::Deque:
		case IR::ContainerType::ForwardList:
		case IR::ContainerType::List:
		case IR::ContainerType::MultiMap:
		case IR::ContainerType::MultiSet:
		case IR::ContainerType::Optional:
		case IR::ContainerType::Pair:
		case IR::ContainerType::PriorityQueue:
		case IR::ContainerType::Queue:
		case IR::ContainerType::Set:
		case IR::ContainerType::Stack:
		case IR::ContainerType::Tuple:
		case IR::ContainerType::UnorderedMap:
		case IR::ContainerType::UnorderedMultiMap:
		case IR::ContainerType::UnorderedMultiSet:
		case IR::ContainerType::UnorderedSet:
		case IR::ContainerType::Valarray:
		case IR::ContainerType::Variant:
			spdlog::error(
			    "Container type {} does not currently have a direct translation via embind. The translation might not work.",
			    Helpers::toString(container));
			break;

		case IR::ContainerType::Allocator:
		case IR::ContainerType::EqualTo:
		case IR::ContainerType::Greater:
		case IR::ContainerType::Hash:
		case IR::ContainerType::Less:
		case IR::ContainerType::SharedPtr:
		case IR::ContainerType::UniquePtr: break;
	}
	return std::nullopt;
}

/**
* Go through type and check if it requires any extra pybind11 includes
* E.g. vector conversion requires inclusion of <pybind11/stl.h>
*/
std::optional<std::string> extractRegisterCommands(IR::Type const& type) {
	if (auto container = Helpers::getContainer(type)) {
		if (auto s = getRegisterString(container->m_container)) {
			auto registerCmd =
			    fmt::format(s.value(),
			                Builders::getTemplateParameters(type),
			                Builders::buildTypeString(type, "_"));
			if (container->m_container == IR::ContainerType::Array) {
				for (int i = 0; i < std::stoi(Builders::extractArraySize(
				                        type.m_representation));
				     ++i) {
					// See https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#value-types
					registerCmd +=
					    fmt::format(".element(emscripten::index<{}>())", i);
				}
			}

			return registerCmd;
		}
	}
	return std::nullopt;
}

bool isSharedPtr(IR::ContainerType c) {
	return c == IR::ContainerType::SharedPtr;
}

std::optional<std::string>
getFirstUserDefinedRepresentation(std::vector<IR::Type> const& types) {
	if (!types.empty()) {
		if (auto userDefined = Helpers::getUserDefined(types[0])) {
			return userDefined->m_representation;
		}
	}
	return std::nullopt;
}

std::optional<std::string> extractShared(IR::Type const& type) {
	if (auto container = Helpers::getContainer(type)) {
		if (isSharedPtr(container->m_container)) {
			return getFirstUserDefinedRepresentation(
			    container->m_containedTypes);
		}
	}
	return std::nullopt;
}

void checkType(IR::Type const& type, EmbindProxy::TypeInfo& info) {
	std::queue<std::reference_wrapper<IR::Type const>> typesToCheck;
	typesToCheck.push(type);

	while (!typesToCheck.empty()) {
		auto& current = typesToCheck.front();

		if (auto registerCmd = extractRegisterCommands(current)) {
			info.m_registerCommands.insert(registerCmd.value());
		}

		if (auto sharedClass = extractShared(current)) {
			info.m_classesMarkedShared.insert(sharedClass.value());
		}

		if (auto container = Helpers::getContainer(current)) {
			for (auto const& containedType : container->m_containedTypes) {
				typesToCheck.push(containedType);
			}
		}
		typesToCheck.pop();
	}
}

}    // namespace Helpers::Embind
