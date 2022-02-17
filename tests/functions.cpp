#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/exportAsExample.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Write to file functions", "[functions]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

int sayTen() {
	return 10;
}

std::string giveBack(std::string const& s) {
	return s;
}

namespace MyNamespace {
	int add(int x, int y) {
		return x + y;
	}
	namespace Nested {
		int increase(int x) {
			return x + 1;
		}
	}
}
)";

	auto jsTestCode = R"(
expect(m.sayTen()).toBe(10);

expect(m.giveBack("hello")).toBe("hello");

expect(m.MyNamespace_add(1, 2)).toBe(3);
expect(m.MyNamespace_Nested_increase(2)).toBe(3);
)";

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, jsTestCode, moduleName);
	REQUIRE(errorCode == 0);

	using Code = TestUtil::Code;
	TestUtil::exportAsExample(
	    "Functions",
	    {Code {"cpp", cppCode}, Code {"javascript", jsTestCode}},
	    TestStage::getExamplesPath());
}
