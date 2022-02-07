#include "Builders/enumBuilder.hpp"
#include <string>

namespace Builders {

EmbindProxy::Enum buildEnum(IR::Enum const& e) {
	EmbindProxy::Enum proxyEnum(e.m_name, e.m_representation);

	proxyEnum.setScoped(e.m_isScoped);

	for (auto const& value : e.m_values) {
		proxyEnum.addValue(value);
	}

	return proxyEnum;
}
}    // namespace Builders
