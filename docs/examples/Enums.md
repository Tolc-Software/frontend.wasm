
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
