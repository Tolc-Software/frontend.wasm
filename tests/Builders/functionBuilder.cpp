#include "Builders/functionBuilder.hpp"
#include <catch2/catch.hpp>

TEST_CASE("function returning containers adds the correct include", "[functionBuilder]") {
	IR::Function f;
	IR::Type t;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	t.m_type = c;
	f.m_returnType = t;
	EmbindProxy::TypeInfo typeInfo;
	auto jsFunction = Builders::buildFunction(f, typeInfo).value();
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "<pybind11/stl.h>");
	}
}
