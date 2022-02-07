#pragma once

#include "EmbindProxy/function.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Transforms a IR::Function to a EmbindProxy::Function
*/
std::optional<EmbindProxy::Function>
buildFunction(IR::Function const& fun, EmbindProxy::TypeInfo& typeInfo);
}    // namespace Builders
