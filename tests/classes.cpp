#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/exportAsExample.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/runEmbindTest.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Classes", "[classes]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);
	// Add instantiation in a source file.
	// This cannot be just declared, must be instantiated
	// See https://en.cppreference.com/w/cpp/language/static
	//
	// Instantiation (must be in a source file):
	stage.addModuleFile("test.cpp", "int const WithConstructor::i;");

	auto cppCode = R"(
#include <string>
#include <string_view>

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}

	// There is a separate .cpp file containing
	// int const WithConstructor::i;
	// To initialize i
	static int const i = 5;

	std::string getS() { return m_s; }

	static int getStatic() { return 55; }

private:
	std::string m_s;
};

struct WithMembers {
	int const i = 5;
	std::string s = "hello";
};

class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};

class WithPrivateFunction {
	double multiply(double i, double j) {
		return i * j;
	}
};

namespace MyNamespace {
	struct Nested {
		int const i = 42;
	};
}


)";

	auto jsTestCode = R"(
// Static function
expect(m.WithConstructor.getStatic()).toBe(55);
// Static variable
expect(m.WithConstructor.i).toBe(5);

var withConstructor = new m.WithConstructor("Hello");
expect(withConstructor.getS()).toBe("Hello");

// Classes need to be deleted manually
withConstructor.delete();

var withMembers = new m.WithMembers();

expect(withMembers.i).toBe(5);
try {
	withMembers.i = 10;
} catch (err) {
	expect(err.toString()).toMatch(/BindingError: WithMembers.i is a read-only property/i);
}

expect(withMembers.s).toBe("hello");

withMembers.delete();

var withFunction = new m.WithFunction();

expect(withFunction.add(5, 10)).toBe(15);

withFunction.delete();

var withPrivateFunction = new m.WithPrivateFunction();

try {
	withPrivateFunction.multiply(5, 10);
} catch (err) {
	expect(err.toString()).toMatch(/TypeError: withPrivateFunction.multiply is not a function/i);
}

withPrivateFunction.delete();

var nested = new m.MyNamespace_Nested();

expect(nested.i).toBe(42);

nested.delete();
)";

	auto errorCode =
	    TestUtil::runEmbindTest(stage, cppCode, jsTestCode, moduleName);
	REQUIRE(errorCode == 0);

	using Code = TestUtil::Code;
	TestUtil::exportAsExample(
	    "Classes",
	    {Code {"cpp", cppCode}, Code {"javascript", jsTestCode}},
	    TestStage::getExamplesPath());
}
