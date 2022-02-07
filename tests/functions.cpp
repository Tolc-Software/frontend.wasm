#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Write to file functions", "[functions]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
int sayTen() {
	return 10;
}
)";

	auto jsTestCode = fmt::format(R"(
{moduleName}.sayTen()
)",
	                              fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, jsTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
