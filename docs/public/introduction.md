# Wasm with Tolc #

In order for `C++` to be called from `javascript` there has to be an interface level. `tolc` generates this level from your already written `C++` interface.
To be as close to what an engineer would have written, `tolc` generates human readable [`embind11`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind).
This is then compiled to a `.wasm` and a `.js` file that the `javascript` can import as normal.
