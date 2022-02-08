#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Namespace name same as module", "[namespaceSameAsModule]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
namespace defaultModule {

int complexFunction() {
	return 5;
}

})";

	auto pythonTestCode = fmt::format(R"(
result = {moduleName}.{moduleName}.complexFunction()
self.assertEqual(result, 5)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}