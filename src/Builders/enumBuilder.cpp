#include "Builders/enumBuilder.hpp"
#include <string>

namespace Builders {

Embind::Proxy::Enum buildEnum(IR::Enum const& e) {
	Embind::Proxy::Enum proxyEnum(e.m_name, e.m_representation);

	for (auto const& value : e.m_values) {
		proxyEnum.addValue(value);
	}

	return proxyEnum;
}
}    // namespace Builders
