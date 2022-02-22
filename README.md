# frontend.wasm #

**NOTE:** This is a work in progress and will be added to `Tolc` as soon as possible.

`frontend.wasm` is a library that takes an AST defined by [`IR::IR`](https://github.com/Tolc-Software/IntermediateRepresentation) and writes [`embind`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html) code. The idea is to be able to use the C++ code that corresponds to the AST from the web via the generated `embind`.

## Building ##

Here is a general overview:

Requirements:

* CMake
* Emscripten (only for tests)
* conan
* clang
* For the tests:
  * [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) (Compile integration tests)
  * [npm & node](https://nodejs.org/en/download/) (Used to run parts of integration tests with [Jest](https://jestjs.io/))

Configure the project:

```shell
$ cmake -S. -Bbuild -G Ninja -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DCMAKE_BUILD_TYPE="Debug" -DENABLE_TESTING=ON -DENABLE_PACKAGING=ON
```

Build `tolc`:

```shell
$ cmake --build build
```

Test with `ctest`:

```shell
$ cd build
$ ctest --output-on-failure
```

Install with `CPack`:

```shell
$ cpack -G TGZ --config build/CPackConfig.cmake
```
