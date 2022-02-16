#pragma once

#include "EmbindProxy/typeInfo.hpp"
#include <string>

namespace Helpers::Embind {

/**
  * Creates a getter and a setter to add to TypeInfo for a class
  * Returns {getter, setter} strings how to refer to them
  * This accounts for them being under the namespace specified by TypeInfo
  */
std::pair<std::string, std::string>
createGetterSetter(std::string const& className,
                   std::string const& fullyQualifiedClassName,
                   std::string const& variableName,
                   EmbindProxy::TypeInfo& typeInfo);

/**
  * Creates a getter and a setter to add to TypeInfo for a tuple
  * Returns {getter, setter} strings how to refer to them
  * This accounts for them being under the namespace specified by TypeInfo
  */
std::pair<std::string, std::string>
createGetterSetter(std::string const& typeString,
                   std::string const& representation,
                   std::string const& elementType,
                   int index,
                   EmbindProxy::TypeInfo& typeInfo);
}
