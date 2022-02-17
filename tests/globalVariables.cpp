#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/exportAsExample.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Global variables are converted",
          "[globalVariables]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string_view>

int const i = 0;
double const d = 55;
std::string_view const s = "Hello world";
const char* c = "Hello world";

namespace MyNamespace {
	int const i = 5;
}
)";

	auto jsTestCode = R"(
expect(m.i).toBe(0);

expect(m.d).toBe(55);

expect(m.s).toBe("Hello world");

expect(m.c).toBe("Hello world");

expect(m.MyNamespace_i).toBe(5);
)";

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, jsTestCode, moduleName);
	REQUIRE(errorCode == 0);

	using Code = TestUtil::Code;
	TestUtil::exportAsExample(
	    "Global Variables",
	    {Code {"cpp", cppCode}, Code {"javascript", jsTestCode}},
	    TestStage::getExamplesPath());
}
