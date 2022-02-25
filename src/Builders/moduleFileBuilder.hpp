#pragma once

#include "Embind/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
*/
std::optional<Embind::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& moduleName);
}    // namespace Builders
