#include "EmbindProxy/moduleFile.hpp"
#include "EmbindProxy/function.hpp"
#include "EmbindProxy/module.hpp"
#include "TestUtil/string.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("ModuleFile includes embind and default start of bindings",
          "[moduleFile]") {
	std::string moduleName = "myModule";
	EmbindProxy::Module m(moduleName);
	EmbindProxy::ModuleFile mf(m, moduleName);

	auto embindCode = mf.getEmbind();
	CAPTURE(embindCode);

	// The include
	REQUIRE(TestUtil::contains(embindCode, "#include <emscripten/bind.h>"));

	// The default start of export
	REQUIRE(TestUtil::contains(
	    embindCode, fmt::format("EMSCRIPTEN_BINDINGS({})", moduleName)));
}

// TEST_CASE("ModuleFile can take a module", "[moduleFile]") {
// 	std::string moduleName = "myModule";
// 	EmbindProxy::Module m(moduleName);
// 	EmbindProxy::Function f("f", "f");
// 	m.addFunction(f);
// 	EmbindProxy::ModuleFile mf(m, moduleName);

// 	auto embindCode = mf.getEmbind();
// 	CAPTURE(embindCode);

// 	// The module
// 	REQUIRE(TestUtil::contains(
// 	    embindCode,
// 	    fmt::format("PYBIND11_MODULE({}, {})", moduleName, m.getPrefix())));
// 	// The module contains the function
// 	REQUIRE(TestUtil::contains(embindCode, m.getPrefix() + R"(.def("f", )"));
// }
