#pragma once

#include "Stage/cmakeStage.hpp"
#include <IR/ir.hpp>
#include <filesystem>

namespace TestUtil {
struct EmbindStage {
	EmbindStage(std::filesystem::path const& baseStage,
	            std::string const& moduleName);

	/**
	* Adds the source file moduleName.hpp to the stage with provided content
	* Uses Tolc::Parser to parse the file and returns the global namespace
	*/
	IR::Namespace parseModuleFile(std::string const& content);

	/**
	* Adds a source file to the stage and imports the module file (moduleName.hpp)
	*/
	void addModuleFile(std::filesystem::path const& file,
	                   std::string const& content);

	int runEmbindUnittest(std::string const& testBody);

	/**
	* Run this to keep the stage on the filesystem after the test has completed
	*/
	void keepAliveAfterTest();

	/**
	 * Adds a jest unittest to build directory in the stage
	 */
	void addJestUnitTest(std::string const& body);

	Stage::CMakeStage m_stage;
	std::string m_moduleName;

	std::filesystem::path m_emscripten_toolchain;
};
}    // namespace TestUtil
