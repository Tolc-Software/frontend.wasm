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
		auto [file, content] = m.value();

		stage.addModuleFile(file, content);

		return stage.runEmbindUnittest(jsUnittestCode);
	}

	return 1;
}
}    // namespace TestUtil
