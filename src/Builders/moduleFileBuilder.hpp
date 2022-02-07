#pragma once

#include "EmbindProxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
*/
std::optional<EmbindProxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& moduleName);
}    // namespace Builders
