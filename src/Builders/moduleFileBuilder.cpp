#include "Builders/moduleFileBuilder.hpp"
#include "Builders/moduleBuilder.hpp"
#include "EmbindProxy/moduleFile.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include "Helpers/combine.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
struct ModulePair {
	IR::Namespace const& m_namespace;
	EmbindProxy::Module m_module;
};
}    // namespace

namespace Builders {

std::optional<EmbindProxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	EmbindProxy::TypeInfo typeInfo;

	if (auto maybeRootModule = Builders::buildModule(rootNamespace, typeInfo)) {
		auto rootModule = maybeRootModule.value();
		EmbindProxy::ModuleFile moduleFile(rootModule, rootModuleName);

		std::queue<ModulePair> namespaces;
		for (auto const& subNamespace : rootNamespace.m_namespaces) {
			if (auto m = Builders::buildModule(subNamespace, typeInfo)) {
				namespaces.push({subNamespace, m.value()});
			} else {
				return std::nullopt;
			}
		}

		while (!namespaces.empty()) {
			auto const& [currentNamespace, currentModule] = namespaces.front();

			moduleFile.addModule(currentModule);

			// Go deeper into the nested namespaces
			for (auto const& subNamespace : currentNamespace.m_namespaces) {
				if (auto m = Builders::buildModule(subNamespace, typeInfo)) {
					namespaces.push({subNamespace, m.value()});
				} else {
					return std::nullopt;
				}
			}

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		moduleFile.setTypeInfo(typeInfo);
		return moduleFile;
	}

	return std::nullopt;
}
}    // namespace Builders
