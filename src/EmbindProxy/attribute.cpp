#include "EmbindProxy/attribute.hpp"
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

Attribute::Attribute(std::string const& name,
                     std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName) {}

std::string Attribute::getEmbind(std::string const& namePrefix) const {
	return fmt::format(R"(em::constant("{name}", {fullyQualifiedName}))",
	                   fmt::arg("name", createName(namePrefix)),
	                   fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
}

std::string Attribute::getPreJS(std::string const& namePrefix) const {
	if (namePrefix.empty()) {
		// No need to rename the function if there is no prefix to remove
		return "";
	}
	// Renaming the function
	// Expects to be injected where necessary
	return fmt::format(R"(
{baseName}: Module['{globalName}'],
)",
	                   fmt::arg("baseName", m_name),
	                   fmt::arg("globalName", createName(namePrefix)));
}

std::string Attribute::createName(std::string const& namePrefix) const {
	return namePrefix + m_name;
}

}    // namespace EmbindProxy
