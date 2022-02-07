#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Namespaces turn into modules", "[namespaces]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

namespace MyLib {

int complexFunction() {
	return 5;
}

	namespace We {
		namespace Are {
			namespace Going {
				namespace Pretty {
					namespace Deep {
						std::string meaningOfLife() {
							return "42";
						}
					}
				}
			}
		}
	}
}

)";

	auto pythonTestCode = fmt::format(R"(
result = {moduleName}.MyLib.complexFunction()
self.assertEqual(result, 5)

lifeProTips = {moduleName}.MyLib.We.Are.Going.Pretty.Deep.meaningOfLife()
self.assertEqual(lifeProTips, "42")
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
