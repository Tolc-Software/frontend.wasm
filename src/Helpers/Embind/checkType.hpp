#pragma once

#include "EmbindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <string>

namespace Helpers::Embind {

// Fill TypeInfo related to input type
// Should be called once per type in the whole interface
void checkType(IR::Type const& type, EmbindProxy::TypeInfo& info);

}    // namespace Helpers::Embind
