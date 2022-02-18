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
	Under,
	Uboat,
};

enum class Scoped {
	Sacred,
	Snail,
};

class EnumTest {
public:
	enum class Inside {
		One,
		Two
	};

	explicit EnumTest(Scoped s) : memberEnum(s), inside(Inside::One) {};

	Inside inside;

	Scoped memberEnum;
};

Unscoped echo(Unscoped s) {
	return s;
}

namespace MyNamespace {
	enum class Color {
		Red,
		Green,
		Blue
	};

	struct Carrier {
		enum class Translator {
			Tolc
		};
	};
}

)";

	auto jsTestCode = R"(
// Can be passed as arguments
const snail = m.Scoped.Snail;
const enumTest = new m.EnumTest(snail);
expect(enumTest.memberEnum).toBe(snail);

// Nested enums within classes
expect(enumTest.inside).toBe(m.EnumTest.Inside.One);
enumTest.delete();

// Unscoped enums work exactly the same
const uboat = m.Unscoped.Uboat;
expect(m.echo(uboat)).toBe(uboat);

// Nested enums inside namespaces
const green = m.MyNamespace.Color.Green;
expect(green).toBe(m.MyNamespace.Color.Green);

// Nested enums inside namespaces inside structs
const company = m.MyNamespace.Carrier.Translator.Tolc;
expect(company).toBe(m.MyNamespace.Carrier.Translator.Tolc);
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
