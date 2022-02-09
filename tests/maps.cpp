#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::maps", "[maps]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <map>
#include <string>

std::map<int, std::string> getData() {
	std::map<int, std::string> m;
	m.insert({10, "hello"});
	return m;
}

)";

	auto jsTestCode = R"(
var data = m.getData();

expect(data.size()).toBe(1);

expect(data.get(10)).toBe("hello");

data.set(50, "Stuff");

expect(data.size()).toBe(2);
expect(data.get(50)).toBe("Stuff");
)";

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, jsTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
