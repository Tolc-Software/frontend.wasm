#pragma once
#include <IR/ir.hpp>
#include <string>
#include <vector>

namespace Builders {

/**
* Transforms a IR::Type to a _ separated version of the type
* Ex:
*   int -> int
*   std::vector<int> -> vector_int
*   std::map<int, std::string> -> map_int_string
*   std::map<UserDefined, int> -> map_UserDefined_int
*/
std::string buildTypeString(IR::Type const& t);

/**
* Call buildTypeString on a number of parameters and join them
*/
std::string getTemplateParameterString(std::vector<IR::Type> const& parameters);

}    // namespace Builders
