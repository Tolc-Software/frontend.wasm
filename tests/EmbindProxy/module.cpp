#include "EmbindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Modules defines their functions", "[module]") {
	std::string moduleName = "myModule";
	EmbindProxy::Module m(moduleName);

	std::vector<std::string> functions = {"f", "calculate", "foo"};
	for (auto const& function : functions) {
		m.addFunction(EmbindProxy::Function(function, function));
	}

	auto embindCode = m.getEmbind();
	CAPTURE(embindCode);

	using TestUtil::contains;
	for (auto const& function : functions) {
		auto expectedContains =
		    fmt::format(R"(function("{function}", &{function})",
		                fmt::arg("function", function));
		CAPTURE(function);
		CAPTURE(expectedContains);
		REQUIRE(contains(embindCode, expectedContains));
	}
}

// TEST_CASE("Modules defines their submodules", "[module]") {
// 	std::string moduleName = "myTestModule";
// 	EmbindProxy::Module m(moduleName);

// 	std::vector<std::string> submodules = {"sub1", "mySub", "child"};
// 	for (auto const& submodule : submodules) {
// 		m.addSubmodule(submodule, moduleName + "__" + submodule);
// 	}

// 	auto embindCode = m.getEmbind();
// 	CAPTURE(embindCode);

// 	using TestUtil::contains;
// 	for (auto const& submodule : submodules) {
// 		auto expectedContains = fmt::format(
// 		    R"(auto {moduleName}__{submodule} = {module}.def_submodule("{submodule}");)",
// 		    fmt::arg("moduleName", moduleName),
// 		    fmt::arg("module", m.getPrefix()),
// 		    fmt::arg("submodule", submodule));
// 		CAPTURE(submodule);
// 		CAPTURE(expectedContains);
// 		REQUIRE(contains(embindCode, expectedContains));
// 	}
// }

// TEST_CASE("Modules defines their classes", "[module]") {
// 	std::string moduleName = "myTestModule";
// 	EmbindProxy::Module m(moduleName);

// 	std::vector<std::string> classes = {"Cl", "MyClass", "OtherClass"};
// 	for (auto const& cls : classes) {
// 		EmbindProxy::Class c(cls, cls);
// 		m.addClass(c);
// 	}

// 	auto embindCode = m.getEmbind();
// 	CAPTURE(embindCode);

// 	using TestUtil::contains;
// 	for (auto const& cls : classes) {
// 		auto expectedContains =
// 		    fmt::format("\tpy::class_<{cls}>({moduleName}, \"{cls}\");\n",
// 		                fmt::arg("cls", cls),
// 		                fmt::arg("moduleName", m.getPrefix()));
// 		CAPTURE(cls);
// 		CAPTURE(expectedContains);
// 		REQUIRE(contains(embindCode, expectedContains));
// 	}
// }

// TEST_CASE("Modules gets a somewhat unique variable name", "[module]") {
// 	std::string moduleName = "myTestModule";
// 	std::string variableName = std::string("rootModule__") + moduleName;
// 	EmbindProxy::Module m(variableName);

// 	using TestUtil::contains;
// 	REQUIRE(variableName == "rootModule__myTestModule");
// }

// TEST_CASE("Module variable name", "[module]") {
// 	std::string moduleName = "myTestModule";
// 	std::string variableName = moduleName + "__ns";
// 	EmbindProxy::Module m(variableName);

// 	using TestUtil::contains;
// 	REQUIRE(m.getPrefix() == "myTestModule__ns");
// }
