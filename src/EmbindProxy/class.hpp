#pragma once

#include "EmbindProxy/enum.hpp"
#include "EmbindProxy/function.hpp"
#include <string>
#include <vector>

namespace EmbindProxy {

class Class {
public:
	explicit Class(std::string const& name,
	               std::string const& fullyQualifiedName);

	void addEnum(Enum const& e);

	void addFunction(Function const& function);

	void addConstructor(Function const& constructor);

	void addMemberVariable(std::string const& variableName,
	                       bool isConst,
	                       bool isStatic);

	std::string const& getName() const;

	// Will be managed by a std::shared_ptr on the python side
	// instead of the default std::unique_ptr
	void setAsManagedByShared();

	std::string getEmbind() const;

private:
	struct MemberVariable {
		// User defined name of the member variable
		std::string m_name;
		bool m_isConst;
		bool m_isStatic;
	};

	// User defined name of the class
	std::string m_name;
	std::string m_fullyQualifiedName;

	std::vector<Function> m_constructors;
	std::vector<Function> m_functions;
	std::vector<MemberVariable> m_memberVariables;
	std::vector<Enum> m_enums;

	bool m_isManagedByShared;
};
}    // namespace EmbindProxy
