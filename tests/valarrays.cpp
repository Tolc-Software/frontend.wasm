#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Simple valarrays", "[valarrays]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <valarray>

std::valarray<int> get() {
	return {1, 2, 3};
}
)";

	auto pythonTestCode = fmt::format(R"(
v = {moduleName}.get()
self.assertEqual(v, [1, 2, 3])
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
