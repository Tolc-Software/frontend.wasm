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

// Builds the submodules corresponding to the sub namespaces of currentNamespace
// Adds each built submodule to namespaces
// Returns false on failure
bool tryBuildSubModules(IR::Namespace const& currentNamespace,
                        EmbindProxy::TypeInfo& typeInfo,
                        std::queue<ModulePair>& namespaces,
                        int currentLevel) {
	for (auto const& subNamespace : currentNamespace.m_namespaces) {
		if (auto m = Builders::buildModule(subNamespace, typeInfo)) {
			m.value().setLevel(currentLevel + 1);
			namespaces.push({subNamespace, m.value()});
		} else {
			return false;
		}
	}
	return true;
}

}    // namespace

namespace Builders {

std::optional<EmbindProxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	EmbindProxy::TypeInfo typeInfo;

	if (auto maybeRootModule = Builders::buildModule(rootNamespace, typeInfo)) {
		auto rootModule = maybeRootModule.value();
		int level = 0;
		rootModule.setLevel(level);
		EmbindProxy::ModuleFile moduleFile(rootModule, rootModuleName);

		std::queue<ModulePair> namespaces;
		if (!tryBuildSubModules(rootNamespace, typeInfo, namespaces, level)) {
			return std::nullopt;
		}
		level++;

		while (!namespaces.empty()) {
			auto const& [currentNamespace, currentModule] = namespaces.front();

			moduleFile.addModule(currentModule);

			// Go deeper into the nested namespaces
			if (!tryBuildSubModules(
			        currentNamespace, typeInfo, namespaces, level)) {
				return std::nullopt;
			}
			level++;

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		moduleFile.setTypeInfo(typeInfo);
		return moduleFile;
	}

	return std::nullopt;
}
}    // namespace Builders
