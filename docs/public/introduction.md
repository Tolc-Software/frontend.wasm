# WebAssembly with Tolc #

In order for `C++` to be called from `javascript` there has to be an interface level. `tolc` generates this level from your already written `C++` interface.
To be as close to what an engineer would have written, `tolc` generates human readable [`embind11`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind).
This is then compiled to a `.wasm` and a `.js` file that `javascript` can import.

## Prerequisites ##

The following works on all supported platforms. On all platforms you need `git` available in your `path`. Commands that should be run from a terminal starts with `$ `, while comments starts with `# `.

### Downloading `Tolc` ###

Just add the following in a `CMakeLists.txt` below where the library you intend to use from `javascript` is defined:

```cmake
# Can be ["latest", "v0.2.0", ...]
set(tolc_version latest)
include(FetchContent)
FetchContent_Declare(
  tolc_entry
  URL https://github.com/Tolc-Software/tolc/releases/download/${tolc_version}/tolc-${CMAKE_HOST_SYSTEM_NAME}.tar.xz
)
FetchContent_Populate(tolc_entry)

find_package(
  tolc
  CONFIG
  PATHS
  ${tolc_entry_SOURCE_DIR}
  REQUIRED)

tolc_create_translation(
  TARGET MyLib
  LANGUAGE wasm
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wasm-bindings)
```

Assuming your library is called `MyLib`, and the bindings should be generated to `${CMAKE_CURRENT_BINARY_DIR}/wasm-bindings`.

### Downloading `Emscripten` ###

In order to compile your library to `WebAssembly`, you need to download the [`Emscripten compiler`](https://emscripten.org/). This is typically done via their `Emscripten SDK`. Navigate to the directory where you want to install and run the following commands:

```shell
# Download SDK
$ git clone https://github.com/emscripten-core/emsdk.git
$ cd emsdk
```

Now follow the specifig commands for your platform.

#### Linux/MacOS ####

From within the `emsdk` directory:

```shell
# Download and install locally
$ ./emsdk install 3.1.3
# Writes configuration file .emscripten
$ ./emsdk activate 3.1.3
```

#### Windows ####

From within the `emsdk` directory:

```shell
# Download and install locally
$ emsdk.bat install 3.1.3
# Writes configuration file .emscripten
$ emsdk.bat activate 3.1.3
```

---

Now when configuring your `CMake` project, pass the toolchain flag `-DCMAKE_TOOLCHAIN_FILE=${EMSDK_DIRECTORY}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake`. Where you need to replace `${EMSDK_DIRECTORY}` with the directory of the previously downloaded `Emscripten SDK`.

Example:

```shell
# Configures project to build using Emscripten
$ cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE=${EMSDK_DIRECTORY}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
```

