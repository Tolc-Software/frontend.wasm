
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
