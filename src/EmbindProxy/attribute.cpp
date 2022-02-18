#include "EmbindProxy/attribute.hpp"
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

Attribute::Attribute(std::string const& name,
                     std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName) {}

std::string Attribute::getEmbind(std::string const& namePrefix) const {
	return fmt::format("\tem::constant(\"{name}\", {fullyQualifiedName});\n",
	                   fmt::arg("name", createName(namePrefix)),
	                   fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
}

std::string Attribute::getPreJS(std::string const& namePrefix) const {
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
