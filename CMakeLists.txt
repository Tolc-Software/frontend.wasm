cmake_minimum_required(VERSION 3.15)
project(
  Frontend.wasm
  VERSION 0.1
  LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  cmake_modules
  GIT_REPOSITORY https://github.com/Tolc-Software/cmake-modules.git
  GIT_TAG main)
FetchContent_MakeAvailable(cmake_modules)
# Basis of all shared CMake modules
set(modules ${cmake_modules_SOURCE_DIR}/cmake)

# Set ccache, ipo, etc.
include(${modules}/ProjectSettings.cmake)

include(${modules}/WarningsAndOptions.cmake)

include(${modules}/Sanitizers.cmake)

# Define run_conan
include(${modules}/ConanHelpers.cmake)
# Create the profile from variables known during configure
setup_conan_profile(VARIABLE frontend_conan_profile)
# This helps with packaging
set(FRONTEND.WASM_CONAN_REQUIRES_PACKAGES spdlog/1.8.0 fmt/7.1.3)
find_conan_packages(REQUIRES ${FRONTEND.WASM_CONAN_REQUIRES_PACKAGES} PROFILE
                    ${frontend_conan_profile})

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY git@github.com:Tolc-Software/IntermediateRepresentation.git
  GIT_TAG main)

FetchContent_MakeAvailable(IRSpecification)

add_library(
  Frontend.wasm
  src/Builders/attributeBuilder.cpp
  src/Builders/classBuilder.cpp
  src/Builders/enumBuilder.cpp
  src/Builders/functionBuilder.cpp
  src/Builders/moduleBuilder.cpp
  src/Builders/moduleFileBuilder.cpp
  src/Builders/typeToStringBuilder.cpp
  src/EmbindProxy/attribute.cpp
  src/EmbindProxy/class.cpp
  src/EmbindProxy/enum.cpp
  src/EmbindProxy/function.cpp
  src/EmbindProxy/module.cpp
  src/EmbindProxy/moduleFile.cpp
  src/Frontend/Wasm/frontend.cpp
  src/Helpers/Embind/checkType.cpp
  src/Helpers/Embind/createFunctions.cpp
  src/Helpers/getOverloadedFunctions.cpp
  src/Helpers/split.cpp
  src/Helpers/string.cpp
  src/Helpers/types.cpp)

add_warnings(TARGET Frontend.wasm)
add_options(TARGET Frontend.wasm)

# Set the sanitizer options on project_options
enable_sanitizers(Frontend.wasm)

target_link_libraries(
  Frontend.wasm
  PUBLIC IR::IR
  PRIVATE fmt::fmt)

target_include_directories(Frontend.wasm PRIVATE src)
target_include_directories(
  Frontend.wasm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                     $<INSTALL_INTERFACE:include>)

set_target_properties(
  Frontend.wasm
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF)

# Add library to link against
add_library(Tolc::Frontend.wasm ALIAS Frontend.wasm)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    option(ENABLE_COVERAGE "Use code coverage tools on the tests" OFF)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" OFF)
  if(ENABLE_PACKAGING)
    add_subdirectory(tools/packaging)
  endif()
endif()
