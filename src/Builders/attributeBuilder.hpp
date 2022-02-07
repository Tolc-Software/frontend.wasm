#pragma once

#include "EmbindProxy/attribute.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include <IR/ir.hpp>

namespace Builders {

/**
* Transforms a IR::Variable to a EmbindProxy::Attribute
*/
EmbindProxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v,
                                      EmbindProxy::TypeInfo& typeInfo);
}    // namespace Builders
