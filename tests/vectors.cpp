#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/exportAsExample.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::vectors", "[vectors]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <vector>

std::vector<int> getData() {
	return {0, 1, 2};
}

)";

	auto jsTestCode = R"(
var data = m.getData();

expect(data.size()).toBe(3);

for (var i = 0; i < data.size(); i++) {
    expect(data.get(i)).toBe(i);
}

data.push_back(3);

expect(data.size()).toBe(4);

expect(data.get(3)).toBe(3);
)";

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, jsTestCode, moduleName);
	REQUIRE(errorCode == 0);

	using Code = TestUtil::Code;
	TestUtil::exportAsExample(
	    "Enums",
	    {Code {"cpp", cppCode}, Code {"javascript", jsTestCode}},
	    TestStage::getExamplesPath());
}
