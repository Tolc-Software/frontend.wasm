
## Classes ##


```cpp

#include <string>
#include <string_view>

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}

	static int const i = 5;

	std::string getS() { return m_s; }
	// std::string_view getSView() { return m_s; }

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


```

```javascript

expect(m.WithConstructor.getStatic()).toBe(55);

var withConstructor = new m.WithConstructor("Hello");

expect(withConstructor.getS()).toBe("Hello");
// expect(withConstructor.getSView()).toBe("Hello");
// expect(withConstructor.i).toBe(5);

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

```
