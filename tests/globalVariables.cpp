#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Global variables are converted",
          "[globalVariables]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
#include <string_view>

int const i = 0;
double const d = 55;
std::string_view const s = "Hello world";
const char* c = "Hello world";
)";

	auto jsTestCode = R"(
// # Starts at 0 and can be changed
// self.assertEqual(m.i, 0)
// m.i = 5
// self.assertEqual(m.i, 5)

expect(m.i).toBe(0);
expect(m.d).toBe(55);
expect(m.s).toBe("Hello world");
expect(m.c).toBe("Hello world");
)";

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, jsTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
