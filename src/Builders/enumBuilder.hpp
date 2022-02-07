#pragma once

#include "EmbindProxy/enum.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Enum to a EmbindProxy::Enum
*/
EmbindProxy::Enum buildEnum(IR::Enum const& s);
}    // namespace Builders
