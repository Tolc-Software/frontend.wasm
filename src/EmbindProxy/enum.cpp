#include "EmbindProxy/enum.hpp"
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

Enum::Enum(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_values({}) {};

std::string Enum::getEmbind(std::string const& namePrefix) const {
	std::string out =
	    fmt::format("em::enum_<{fullyQualifiedName}>(\"{namePrefix}{name}\")\n",
	                fmt::arg("namePrefix", namePrefix),
	                fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	                fmt::arg("name", m_name));

	for (auto const& value : m_values) {
		out += fmt::format(
		    "\t\t.value(\"{value}\", {fullyQualifiedName}::{value})\n",
		    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
		    fmt::arg("value", value));
	}

	// Remove the last newline
	out.pop_back();

	return out;
}

void Enum::addValue(std::string const& value) {
	m_values.push_back(value);
}

}    // namespace EmbindProxy
