#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/exportAsExample.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Testing enums", "[enums]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
enum Unscoped {
	A,
	B,
	C
};

enum class Scoped {
	D,
	E,
	F
};

class EnumTest {
public:
	enum class Inside {
		One,
		Two
	};

	explicit EnumTest(Scoped s) : e(s), i(Inside::One) {};

	Inside i;

	Scoped e;
};

Unscoped f(Unscoped s) {
	return s;
}

namespace MyNamespace {
	enum class Color {
		Red,
		Green,
		Blue
	};
}

)";

	auto jsTestCode = R"(
const scoped = m.Scoped.D;

// Can be passed as arguments
var enumTest = new m.EnumTest(scoped);
expect(enumTest.e).toBe(scoped);
enumTest.delete();

// Unscoped enums work exactly the same
unscoped = m.Unscoped.B;
u = m.f(unscoped);
expect(u).toBe(unscoped);

// Nested enums work aswell
const c = m.MyNamespace.Color.Green;
expect(c).toBe(m.MyNamespace.Color.Green);
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
