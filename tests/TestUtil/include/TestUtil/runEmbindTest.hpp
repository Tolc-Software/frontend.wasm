#pragma once

#include "Frontend/Wasm/frontend.hpp"
#include "TestUtil/embindStage.hpp"
#include <iostream>

namespace TestUtil {

int runEmbindTest(TestUtil::EmbindStage& stage,
                  std::string const& cppCode,
                  std::string const& jsUnittestCode,
                  std::string const& moduleName) {
	auto globalNS = stage.parseModuleFile(cppCode);

	if (auto m = Frontend::Wasm::createModule(globalNS, moduleName)) {
		for (auto const& [filename, content] : m.value()) {
			if (filename.string() == "pre.js") {
				stage.m_stage.addFile("src" / filename, content);
			} else {
				stage.addModuleFile(filename, content);
			}

		}
		return stage.runEmbindUnittest(jsUnittestCode);
	}

	return 1;
}
}    // namespace TestUtil
