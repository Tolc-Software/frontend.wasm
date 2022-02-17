#include "EmbindProxy/attribute.hpp"
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

Attribute::Attribute(std::string const& name,
                     std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName) {}

std::string Attribute::getEmbind(std::string const& namePrefix) const {
	return fmt::format(
	    R"(em::constant("{namePrefix}{name}", {fullyQualifiedName}))",
	    fmt::arg("namePrefix", namePrefix),
	    fmt::arg("name", m_name),
	    fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
}
}    // namespace EmbindProxy
