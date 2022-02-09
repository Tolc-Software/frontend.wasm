#include "Builders/typeToStringBuilder.hpp"
#include <TestUtil/types.hpp>
#include <catch2/catch.hpp>
#include <variant>

TEST_CASE("Template parameters", "[typeToStringBuilder]") {
	REQUIRE(Builders::getTemplateParameters(TestUtil::getVector()) == "int");
	REQUIRE(Builders::getTemplateParameters(TestUtil::getMap()) == "int, int");
}

TEST_CASE("Base cases", "[typeToStringBuilder]") {
	REQUIRE(Builders::buildTypeString(TestUtil::getType(), "_") == "int");
	REQUIRE(Builders::buildTypeString(TestUtil::getVector(), "_") ==
	        "vector_int");
	REQUIRE(Builders::buildTypeString(TestUtil::getMap(), "_") ==
	        "map_int_int");
}

TEST_CASE("Nested cases", "[typeToStringBuilder]") {
	auto m = TestUtil::getMap();
	auto c = TestUtil::getContainer(m);
	REQUIRE(c != nullptr);
	c->m_containedTypes.back() = TestUtil::getVector();
	// map<int, vector<int>>
	REQUIRE(Builders::buildTypeString(m, "_") == "map_int_vector_int");
}

TEST_CASE("Multiple types", "[typeToStringBuilder]") {
	auto tuple = TestUtil::getMap();
	auto c = TestUtil::getContainer(tuple);
	REQUIRE(c != nullptr);
	c->m_container = IR::ContainerType::Tuple;
	c->m_containedTypes.push_back(TestUtil::getType());
	c->m_containedTypes.push_back(TestUtil::getType());
	// tuple<int, int, int, int>
	REQUIRE(Builders::buildTypeString(tuple, "_") == "tuple_int_int_int_int");
}

TEST_CASE("Other separators", "[typeToStringBuilder]") {
	auto tuple = TestUtil::getMap();
	auto c = TestUtil::getContainer(tuple);
	REQUIRE(c != nullptr);
	c->m_container = IR::ContainerType::Tuple;
	c->m_containedTypes.push_back(TestUtil::getType());
	c->m_containedTypes.push_back(TestUtil::getType());
	// tuple<int, int, int, int>
	REQUIRE(Builders::buildTypeString(tuple, ", ") ==
	        "tuple, int, int, int, int");
}
