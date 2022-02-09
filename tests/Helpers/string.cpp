#include "Helpers/string.hpp"
#include <catch2/catch.hpp>
#include <string>

TEST_CASE("Trim strings", "[strings]") {
	std::string s = "std::vector<int>";
	auto leftTrimmed = Helpers::trimLeft(s, '<');
	REQUIRE(leftTrimmed == "int>");
	auto rightTrimmed = Helpers::trimRight(s, '>');
	REQUIRE(rightTrimmed == "std::vector<int");
	REQUIRE(Helpers::trimLeft(rightTrimmed, '<') == "int");
}
