#include "Builders/moduleFileBuilder.hpp"
#include "EmbindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE(
    "Adding a function with a vector for argument automaticall includes stl.h",
    "[moduleFileBuilder]") {
	IR::Namespace ns;
	IR::Function f;
	f.m_name = 'f';
	f.m_representation = f.m_name;
	IR::Type returnType;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	returnType.m_type = c;
	f.m_returnType = returnType;
	ns.m_functions.push_back(f);

	std::string moduleName = "MyModule";

	auto file = Builders::buildModuleFile(ns, moduleName).value();
	auto path = file.getFilepath();
	auto embind = file.getEmbind();
	CAPTURE(path);
	CAPTURE(embind);

	REQUIRE(path == "MyModule.cpp");

	for (auto const& expectedContains :
	     {"#include <embind11/embind11.h>",
	      "#include <embind11/stl.h>",
	      "namespace em = pybind11;",
	      "PYBIND11_MODULE(MyModule, MyModule)"}) {
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(embind, expectedContains));
	}
}

TEST_CASE("One level namespace", "[moduleFileBuilder]") {
	IR::Namespace ns;
	std::string moduleName = "MyModule";

	auto file = Builders::buildModuleFile(ns, moduleName).value();
	auto path = file.getFilepath();
	auto embind = file.getEmbind();
	CAPTURE(path);
	CAPTURE(embind);

	REQUIRE(path == "MyModule.cpp");

	for (auto const& expectedContains :
	     {"#include <embind11/embind11.h>",
	      "namespace em = pybind11;",
	      "PYBIND11_MODULE(MyModule, MyModule)"}) {
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(embind, expectedContains));
	}
}

TEST_CASE("Two level namespace", "[moduleFileBuilder]") {
	IR::Namespace ns;

	std::vector subNamespaces = {"sub1", "sub2", "sub3"};
	for (auto const& subNs : subNamespaces) {
		IR::Namespace sub;
		sub.m_name = subNs;
		sub.m_representation = sub.m_name;
		ns.m_namespaces.push_back(sub);
	}

	std::string moduleName = "MyModule";
	auto file = Builders::buildModuleFile(ns, moduleName).value();
	auto path = file.getFilepath();
	auto embind = file.getEmbind();
	CAPTURE(path);
	CAPTURE(embind);

	REQUIRE(path == "MyModule.cpp");

	for (auto const& subNamespace : subNamespaces) {
		auto expectedContains = fmt::format(
		    R"(auto {moduleName}_{subNamespace} = {moduleName}.def_submodule("{subNamespace}");)",
		    fmt::arg("moduleName", moduleName),
		    fmt::arg("subNamespace", subNamespace));
		CAPTURE(subNamespace);
		CAPTURE(expectedContains);

		REQUIRE(TestUtil::contains(embind, expectedContains));
	}
}

TEST_CASE("Three level namespace", "[moduleFileBuilder]") {
	IR::Namespace ns;

	IR::Namespace sub;
	sub.m_name = "sub";
	sub.m_representation = sub.m_name;

	IR::Namespace subsub;
	subsub.m_name = "subsub";
	subsub.m_representation = sub.m_representation + "::" + subsub.m_name;

	// Add them in reverse order
	sub.m_namespaces.push_back(subsub);
	ns.m_namespaces.push_back(sub);

	std::string moduleName = "MyModule";
	auto file = Builders::buildModuleFile(ns, moduleName).value();
	auto embind = file.getEmbind();
	CAPTURE(embind);

	auto firstSub = R"(auto MyModule_sub = MyModule.def_submodule("sub");)";

	REQUIRE(TestUtil::contains(embind, firstSub));

	auto secondSub =
	    R"(auto MyModule_sub_subsub = MyModule_sub.def_submodule("subsub");)";
	REQUIRE(TestUtil::contains(embind, secondSub));
}
