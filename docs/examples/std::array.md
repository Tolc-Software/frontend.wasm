
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
