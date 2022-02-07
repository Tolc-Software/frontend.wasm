#pragma once

#include "Builders/functionBuilder.hpp"
#include "EmbindProxy/module.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<EmbindProxy::Module>
buildModule(IR::Namespace const& ns,
            EmbindProxy::TypeInfo& typeInfo);
}    // namespace Builders
