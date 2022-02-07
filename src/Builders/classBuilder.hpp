#pragma once

#include "EmbindProxy/class.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Builders {

/**
* Transforms a IR::Struct to a EmbindProxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
* Fails if any member functions takes unique_ptr as an argument
*/
std::optional<EmbindProxy::Class> buildClass(IR::Struct const& s,
                                             EmbindProxy::TypeInfo& typeInfo);
}    // namespace Builders
