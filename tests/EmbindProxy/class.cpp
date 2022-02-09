#include "EmbindProxy/class.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Empty class", "[class]") {
	std::string className = "myClass";
	EmbindProxy::Class c(className, className);
	auto embindCode = c.getEmbind();
	CAPTURE(embindCode);

	using TestUtil::contains;
	auto expectedContains = fmt::format(R"(class_<{className}>("{className}"))",
	                                    fmt::arg("className", className));
	CAPTURE(expectedContains);
	REQUIRE(contains(embindCode, expectedContains));
}

TEST_CASE("Class with functions", "[class]") {
	std::string className = "myOtherClass";
	EmbindProxy::Class c(className, className);

	std::vector<std::string> functions = {"f", "calculate", "foo"};
	for (auto const& function : functions) {
		c.addFunction(EmbindProxy::Function(function, function));
	}

	auto embindCode = c.getEmbind();
	CAPTURE(embindCode);
	std::string classDecl =
	    fmt::format("class_<{}>(\"{}\")", className, className);
	CAPTURE(classDecl);

	using TestUtil::contains;

	REQUIRE(contains(embindCode, classDecl));

	for (auto const& function : functions) {
		auto expectedContains =
		    fmt::format("\t\t.function(\"{function}\", &{function}",
		                fmt::arg("function", function));
		CAPTURE(function);
		CAPTURE(expectedContains);
		REQUIRE(contains(embindCode, expectedContains));
	}
}

TEST_CASE("Class with constructor", "[class]") {
	std::string className = "myFreshClass";
	EmbindProxy::Class c(className, className);

	std::vector<std::string> arguments = {"const std::string&", "int"};
	auto constructor = EmbindProxy::Function(className, className);
	for (auto const& argument : arguments) {
		constructor.addArgument(argument);
	}
	constructor.setAsConstructor();
	c.addConstructor(constructor);

	auto embindCode = c.getEmbind();
	CAPTURE(embindCode);

	using TestUtil::contains;
	auto expectedContains =
	    fmt::format("\t\t.constructor<{}>()", fmt::join(arguments, ", "));
	CAPTURE(expectedContains);
	REQUIRE(contains(embindCode, expectedContains));
}

TEST_CASE("Class with member variables", "[class]") {
	std::string className = "SuperbClass";
	EmbindProxy::Class c(className, className);

	std::vector<std::string> constVariables = {"myInt", "var", "yes"};
	for (auto const& variable : constVariables) {
		c.addMemberVariable(variable, true, false);
	}

	std::vector<std::string> nonConstVariables = {"myOtherInt", "var2", "no"};
	for (auto const& variable : nonConstVariables) {
		c.addMemberVariable(variable, false, false);
	}

	auto embindCode = c.getEmbind();
	CAPTURE(embindCode);

	using TestUtil::contains;
	for (auto const& variable : constVariables) {
		auto expectedProperty = fmt::format("\t\t.property(\"{variable}\", ",
		                                    fmt::arg("variable", variable));

		auto expectedGetter =
		    fmt::format("[](auto& _tolc_c) {{ return _tolc_c.{variable}; }})",
		                fmt::arg("variable", variable));

		CAPTURE(expectedProperty);
		CAPTURE(expectedGetter);
		REQUIRE(contains(embindCode, expectedProperty));
		REQUIRE(contains(embindCode, expectedGetter));
	}

	for (auto const& variable : nonConstVariables) {
		auto expectedProperty = fmt::format("\t\t.property(\"{variable}\", ",
		                                    fmt::arg("variable", variable));

		auto expectedGetter =
		    fmt::format("[](auto& _tolc_c) {{ return _tolc_c.{variable}; }}",
		                fmt::arg("variable", variable));

		auto expectedSetter = fmt::format(
		    "[](auto& _tolc_c, auto const& _tolc_v) {{ _tolc_c.{variable} = _tolc_v; }})",
		    fmt::arg("variable", variable));

		CAPTURE(expectedProperty);
		CAPTURE(expectedGetter);
		CAPTURE(expectedSetter);
		REQUIRE(contains(embindCode, expectedProperty));
		REQUIRE(contains(embindCode, expectedGetter));
		REQUIRE(contains(embindCode, expectedSetter));
	}
}

// TEST_CASE("Class with enum", "[class]") {
// 	std::string className = "myFreshClass";
// 	EmbindProxy::Class c(className, className);

// 	std::string enumName = "MyEnum";
// 	std::string fullyQualifiedName = moduleName + "::" + enumName;
// 	EmbindProxy::Enum e(enumName, fullyQualifiedName);
// 	e.setScoped(false);
// 	e.addValue("Hi");
// 	c.addEnum(e);

// 	using TestUtil::contains;
// 	auto expectedContains = fmt::format(
// 	    R"(py::enum_<{fullyQualifiedName}>({className}, "{enumName}")",
// 	    fmt::arg("fullyQualifiedName", fullyQualifiedName),
// 	    fmt::arg("enumName", enumName),
// 	    fmt::arg("className", className));
// 	auto pybind = c.getEmbind();
// 	CAPTURE(pybind);
// 	CAPTURE(expectedContains);
// 	REQUIRE(TestUtil::contains(pybind, expectedContains));
// 	// Require exporting since it is scoped
// 	std::string exportValues = "\t.export_values()";
// 	CAPTURE(exportValues);
// 	REQUIRE(TestUtil::contains(pybind, exportValues));
// }

