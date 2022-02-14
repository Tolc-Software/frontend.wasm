# Examples #

Each example is is made up of a `C++` section and how it can be used from `javascript`. Note that the examples are part of the test suite for `Tolc`, so the `javascript` uses statements from the test framework [`jest`](https://jestjs.io/). You may assume that each test passes if you run the latest version of `Tolc`.



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

## Enums ##


```cpp

#include <vector>

std::vector<int> getData() {
	return {0, 1, 2};
}


```

```javascript

var data = m.getData();

expect(data.size()).toBe(3);

for (var i = 0; i < data.size(); i++) {
    expect(data.get(i)).toBe(i);
}

data.push_back(3);

expect(data.size()).toBe(4);

expect(data.get(3)).toBe(3);

```

## Functions ##


```cpp

#include <string>

int sayTen() {
	return 10;
}

std::string giveBack(std::string const& s) {
	return s;
}

```

```javascript

expect(m.sayTen()).toBe(10);

expect(m.giveBack("hello")).toBe("hello");

```

## Globals ##


```cpp

#include <string_view>

int const i = 0;
double const d = 55;
std::string_view const s = "Hello world";
const char* c = "Hello world";

```

```javascript

expect(m.i).toBe(0);
expect(m.d).toBe(55);
expect(m.s).toBe("Hello world");
expect(m.c).toBe("Hello world");

```

## std::array ##


```cpp

#include <array>
#include <string>

std::array<double, 3> getData3() {
	return {0.0, 1.0, 2.0};
}

// Multiple array types at the same time
std::array<int, 2> getData2() {
	return {0, 1};
}


```

```javascript

var data3 = m.getData3();

// It's just a normal JS array
expect(data3.length).toBe(3);

expect(data3).toStrictEqual([0, 1, 2]);

var data2 = m.getData2();
expect(data2.length).toBe(2);

expect(data2).toStrictEqual([0, 1]);

```

## std::map ##


```cpp

#include <map>
#include <string>

std::map<int, std::string> getData() {
	std::map<int, std::string> m;
	m.insert({10, "hello"});
	return m;
}


```

```javascript

var data = m.getData();

expect(data.size()).toBe(1);

expect(data.get(10)).toBe("hello");

data.set(50, "Stuff");

expect(data.size()).toBe(2);
expect(data.get(50)).toBe("Stuff");

```
