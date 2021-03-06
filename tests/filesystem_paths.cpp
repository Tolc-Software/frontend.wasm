#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("std::filesystem::path gets converted to pathlib.Path",
          "[filesystem_paths]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <filesystem>
#include <vector>

std::filesystem::path takingPath(std::filesystem::path p) {
	return p;
}

std::string toString(std::filesystem::path p) {
	return p.string();
}

std::filesystem::path joinPaths(std::vector<std::filesystem::path> arrayToSum) {
	std::filesystem::path sum;
	for (auto f : arrayToSum) {
		sum /= f;
	}
	return sum;
}
)";

	auto pythonTestCode = fmt::format(R"(
from pathlib import Path

p0 = Path("Hello")
result0 = {moduleName}.takingPath(p0)
self.assertEqual(result0, p0)

p1 = Path("Something")
toString = {moduleName}.toString(p1)
self.assertEqual(toString, p1.name)

result1 = {moduleName}.joinPaths([p0, p1])
self.assertEqual(result1, p0 / p1)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runEmbindTest(cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
