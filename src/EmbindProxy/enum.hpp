#pragma once

#include <string>
#include <vector>

namespace EmbindProxy {
class Enum {
public:
	Enum(std::string const& name, std::string const& fullyQualifiedName);

	/**
	* Creates a string corresponding to the embind conversion of this enum.
	* E.g.
	*   enum_<Pet::Kind>("Kind").value("Dog", Pet::Kind::Dog)
	*/
	std::string getEmbind(std::string const& namePrefix = "") const;

	/**
	* Add an enum value. Should be just the value name.
	* E.g.
	*   enum class MyEnum {
	*      MyValue;
	*   };
	*   Should only add the value 'MyValue' and not 'MyEnum::MyValue'
	*/
	void addValue(std::string const& value);

private:
	// The user defined name of the enum
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::vector<std::string> m_values;
};
}    // namespace EmbindProxy
