#include "TestUtil/embindStage.hpp"
#include "Stage/cmakeStage.hpp"
#include "TestStage/stageFunctions.hpp"
#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <vector>

namespace TestUtil {

EmbindStage::EmbindStage(std::filesystem::path const& baseStage,
                         std::string const& moduleName)
    : m_stage(baseStage / "EmscriptenStage",
              {"cmake", "CMakeLists.txt", "configureAndBuild.bat"}),
      m_moduleName(moduleName) {
	m_stage.setTargetName(m_moduleName);
	m_stage.setWindowsCMakeBuildAndConfigureScript("configureAndBuild.bat");

	m_emscripten_toolchain = baseStage / "build" / "_deps" / "emsdk_entry-src" /
	                         "upstream" / "emscripten" / "cmake" / "Modules" /
	                         "Platform" / "Emscripten.cmake";
}

void EmbindStage::addModuleFile(std::filesystem::path const& file,
                                std::string const& content) {
	m_stage.addSourceFile(file,
	                      "#include \"" + m_moduleName + ".hpp\"\n" + content);
}

IR::Namespace EmbindStage::parseModuleFile(std::string const& content) {
	auto testFile = m_stage.addSourceFile(m_moduleName + ".hpp", content);
	return TestUtil::parseFile(testFile.string());
}

int EmbindStage::runEmbindUnittest(std::string const& testBody) {
	// Make sure the toolchain file exists
	if (!std::filesystem::exists(m_emscripten_toolchain)) {
		std::cerr << "The toolchain file for Emscripen is not found: "
		          << m_emscripten_toolchain.string() << '\n';
		return 1;
	}

	std::string configureCmd =
	    fmt::format("cmake -S. -Bbuild -GNinja -DCMAKE_TOOLCHAIN_FILE={}",
	                m_emscripten_toolchain.string());
	std::string buildCmd = "cmake --build build";
	for (auto const& cmd : {configureCmd, buildCmd}) {
		if (auto setupError = m_stage.runCommand(cmd); setupError != 0) {
			return setupError;
		}
	}

	return 1;

	std::vector<std::string> includes = {m_moduleName};
	std::string testName = "default";

	// Store all the lines in the test
	std::vector<std::string> body;
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = testBody.find('\n', prev)) != std::string::npos) {
		body.push_back(testBody.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// To get the last substring (or only, if delimiter is not found)
	body.push_back(testBody.substr(prev));

	TestStage::addPythonUnittest(
	    m_stage.m_stage, m_moduleName, testName, body, includes);

	return TestStage::runEmbindUnittest(m_stage.m_stage, m_moduleName);
}

void EmbindStage::keepAliveAfterTest() {
	m_stage.m_removeOnDestruction = false;
}
}    // namespace TestUtil
