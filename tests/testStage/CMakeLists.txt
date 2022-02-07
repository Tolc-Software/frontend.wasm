cmake_minimum_required(VERSION 3.15)

project(
  TestStage
  VERSION 1.0
  LANGUAGES CXX)

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/GetEmscripten.cmake)

if(NOT emsdk_SOURCE_DIR)
  get_emscripten(VERSION 2.9.0)
endif()

# Get the variable 'target_name'
# NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# Get the variable 'sources'
# NOTE: This will be generated for each test
include(cmake/Sources.cmake)

# Create the python module
add_emscripten_library(TARGET ${target_name} SOURCES ${sources} EMSDK_PATH
                       ${emsdk_SOURCE_DIR})
# target_include_directories(${target_name} PRIVATE src)

# Use C++17 since the tests are using std::optional etc.
# set_target_properties(
#   ${target_name}
#   PROPERTIES CXX_STANDARD_REQUIRED ON
#              CXX_EXTENSIONS OFF
#              CXX_STANDARD 17)
