#include <emscripten/bind.h>
#include <string>
#include <string_view>
#include <vector>

class MyClass {
public:
	MyClass(int x, std::string y) : x(x), y(y) {}

	void incrementX() {
		++x;
	}

	int getX() const {
		return x;
	}
	void setX(int x_) {
		x = x_;
	}

	static std::string getStringFromInstance(const MyClass& instance) {
		return instance.y;
	}

private:
	int x;
	std::string y;
};

std::vector<std::string> f(std::vector<int> v) {
	std::vector<std::string> out;
	for (auto const& e : v) {
		out.push_back(std::to_string(e));
	}
	return out;
}

// Globals
const std::string_view s = "Hello world";
const std::string s2 = "Hello world";
const char* sc = "Hello world";

// Binding code
EMSCRIPTEN_BINDINGS(my_class_example) {
	emscripten::class_<MyClass>("MyClass")
	    .constructor<int, std::string>()
	    .function("incrementX", &MyClass::incrementX)
	    .property("x", &MyClass::getX, &MyClass::setX)
	    .class_function("getStringFromInstance",
	                    &MyClass::getStringFromInstance);

	emscripten::function("f", &f);
	emscripten::register_vector<int>("vector<int>");
	emscripten::register_vector<std::string>("vector<string >");

	// Global strings are complicated:
	// Does not show
	em::constant("char_p_clean", sc);
	em::constant("string_view_clean", s);

	// Results in empty string
	em::constant("string_clean", s2);

	// Throws on load
	// em::constant("string_view_emval", em::val(s));
	// em::constant("string_emval", em::val(s2));
	// em::constant("string_emval_data", em::val(s2.data()));
	// em::constant("string_emval_c_str", em::val(s2.c_str()));

	// Works
	em::constant("char_p_emval", em::val(sc));
	em::constant("string_view_emval_data", em::val(s.data()));
}
