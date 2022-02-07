#include "Builders/attributeBuilder.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include "Helpers/Embind/checkType.hpp"
#include <string>

namespace Builders {

EmbindProxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v,
                                      EmbindProxy::TypeInfo& typeInfo) {
	EmbindProxy::Attribute attr(v.m_name, parentNamespace + "::" + v.m_name);
	Helpers::Embind::checkType(v.m_type, typeInfo);

	return attr;
}
}    // namespace Builders
