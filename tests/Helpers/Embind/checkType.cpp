#include "Helpers/Embind/checkType.hpp"
#include "Builders/typeToStringBuilder.hpp"
#include "EmbindProxy/typeInfo.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <TestUtil/parse.hpp>
#include <TestUtil/types.hpp>
#include <catch2/catch.hpp>
#include <iostream>

TEST_CASE("Construct register_vector", "[checkType]") {
	auto c = TestUtil::getVector();
	EmbindProxy::TypeInfo typeInfo;
	Helpers::Embind::checkType(c, typeInfo);
	REQUIRE(typeInfo.m_registerCommands.size() == 1);
	for (auto const& command : typeInfo.m_registerCommands) {
		REQUIRE(command == "em::register_vector<int>(\"vector_int\")");
	}
}

TEST_CASE("Construct register_map", "[checkType]") {
	auto c = TestUtil::getMap();
	EmbindProxy::TypeInfo typeInfo;
	Helpers::Embind::checkType(c, typeInfo);
	REQUIRE(typeInfo.m_registerCommands.size() == 1);
	for (auto const& command : typeInfo.m_registerCommands) {
		REQUIRE(command == "em::register_map<int, int>(\"map_int_int\")");
	}
}

TEST_CASE("Construct value_array", "[checkType]") {
	auto c = TestUtil::getArray();
	EmbindProxy::TypeInfo typeInfo;
	Helpers::Embind::checkType(c, typeInfo);
	REQUIRE(typeInfo.m_registerCommands.size() == 1);
	for (auto const& command : typeInfo.m_registerCommands) {
		REQUIRE(TestUtil::contains(
		    command, "em::value_array<std::array<int, 2>>(\"array_int_2\")"));
		REQUIRE(
		    TestUtil::contains(command, ".element(emscripten::index<0>())"));
		REQUIRE(
		    TestUtil::contains(command, ".element(emscripten::index<1>())"));
	}
}

TEST_CASE("Construct value_object for pair", "[checkType]") {
	auto c = TestUtil::getPair();
	EmbindProxy::TypeInfo typeInfo;
	Helpers::Embind::checkType(c, typeInfo);
	REQUIRE(typeInfo.m_registerCommands.size() == 1);
	for (auto const& command : typeInfo.m_registerCommands) {
		CAPTURE(command);
		REQUIRE(TestUtil::contains(
		    command,
		    "em::value_object<std::pair<int, std::string>>(\"pair_int_string\")"));
		REQUIRE(TestUtil::contains(
		    command,
		    R"(.field("first", &std::pair<int, std::string>::first))"));
		TestUtil::contains(
		    command,
		    R"(.field("second", &std::pair<int, std::string>::second))");
	}
}

