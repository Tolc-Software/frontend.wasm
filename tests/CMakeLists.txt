include_guard()

include(${modules}/GetParser.cmake)
get_parser(VERSION v0.2.0)

# Set the include path for the system library in the variable
# We are using the standard library shipped
# with the downloaded llvm for include paths in the parsing
get_parser_system_include(
  VARIABLE
  SYSTEM_INCLUDE_PATHS
  PARSER_SOURCE_DIR
  ${parser_SOURCE_DIR}
  LIBCPP_ROOT_DIR
  ${parser_SOURCE_DIR}/lib/Parser
  LLVM_VERSION
  ${PARSER_LLVM_VERSION})

# If Parser was built from source, the file should only be expanded with empty strings
# and therefore use the default config from Parser.
configure_file(
  TestUtil/include/TestUtil/parserConfig.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestUtil/parserConfig.hpp @ONLY)

include(FetchContent)
FetchContent_Declare(
  stage_content
  GIT_REPOSITORY https://github.com/Tolc-Software/Stage.git
  GIT_TAG main)
FetchContent_MakeAvailable(stage_content)

# Defines create_catch2_test and get_catch2
include(${modules}/Catch2Helpers.cmake)

get_catch2(VERSION v2.13.8)

# Create a platform independent way of getting the path to the test stage
set(test_stage ${CMAKE_CURRENT_LIST_DIR}/testStage)
set(examples_dir ${PROJECT_SOURCE_DIR}/docs/examples)
set(npm_exe npm)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  # This is to normalize strings relative to how they are used in std::filesystem
  string(REPLACE "/" "\\\\" test_stage ${test_stage})
  string(REPLACE "/" "\\\\" examples_dir ${examples_dir})
  # On windows, the actual command that will be executed is npm.cmd
  # https://stackoverflow.com/questions/47224093/ctest-gives-not-run-and-bad-command-although-commands-work-directly
  set(npm_exe npm.cmd)
endif()
configure_file(
  TestUtil/include/TestStage/paths.cpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestStage/paths.cpp @ONLY)

# Everything put in TestUtil/include is available
add_library(
  TestUtil
  TestUtil/include/TestUtil/embindStage.cpp TestUtil/include/TestUtil/parse.cpp
  TestUtil/include/TestStage/paths.cpp)
target_include_directories(TestUtil PUBLIC TestUtil/include)
target_link_libraries(TestUtil PRIVATE Tolc::Frontend.wasm Tolc::Parser
                                       Catch2::Catch2 Tolc::Stage)
target_link_libraries(TestUtil PUBLIC IR::IR)
find_package(fmt REQUIRED)

add_warnings(TARGET TestUtil)
add_options(TARGET TestUtil)

if(ENABLE_COVERAGE)
  include(${modules}/CodeCoverage.cmake)

  # Use coverage of frontend
  target_code_coverage(Frontend.wasm)

  add_code_coverage_all_targets()
endif()

# Before configuring the stage, we have to download node dependencies
add_test(
  NAME NpmInstall
  WORKING_DIRECTORY ${test_stage}/EmscriptenStage
  COMMAND ${npm_exe} install)
set_tests_properties(NpmInstall PROPERTIES FIXTURES_SETUP PreTestStage)

# Create a test fixture for downloading dependencies in the test stage
add_test(
  NAME ConfigureTestStage
  WORKING_DIRECTORY ${test_stage}
  COMMAND ${CMAKE_COMMAND} -S. -Bbuild -GNinja)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_SETUP TestStage)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_REQUIRED
                                                   PreTestStage)

foreach(
  test
  Builders/attributeBuilder.cpp
  Builders/classBuilder.cpp
  Builders/enumBuilder.cpp
  Builders/functionBuilder.cpp
  Builders/moduleFileBuilder.cpp
  Builders/typeToStringBuilder.cpp
  Helpers/Embind/checkType.cpp
  Helpers/combine.cpp
  Helpers/string.cpp
  EmbindProxy/attribute.cpp
  EmbindProxy/class.cpp
  EmbindProxy/enum.cpp
  EmbindProxy/function.cpp
  EmbindProxy/module.cpp
  EmbindProxy/moduleFile.cpp
  arrays.cpp
  classes.cpp
  # complex.cpp
  # deques.cpp
  enums.cpp
  # filesystem_paths.cpp
  # functional.cpp
  functions.cpp
  globalVariables.cpp
  # lists.cpp
  maps.cpp
  namespaceRenaming.cpp
  namespaces.cpp
  # optionals.cpp
  # overloadedFunctions.cpp
  pairs.cpp
  # sets.cpp
  smartPointers.cpp
  # templates.cpp
  tuples.cpp
  # unordered_maps.cpp
  # unordered_sets.cpp
  # userDefined.cpp
  # valarrays.cpp
  # variants.cpp
  vectors.cpp)
  # Ex: EmbindProxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  create_catch2_test(
    TARGET
    ${testName}
    SOURCES
    ${test}
    INCLUDE
    ${PROJECT_SOURCE_DIR}/src
    LINK_LIBRARIES
    Frontend.wasm
    TestUtil
    Tolc::Stage
    fmt::fmt)

  add_warnings(TARGET ${testName})
  add_options(TARGET ${testName})

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})

  string(FIND "${test}" "/" subdirectory)
  if(subdirectory EQUAL -1)
    # The test is in the root => should be an integration test

    # Require that the dependencies are downloaded so they can be copied to a
    # temporary test stage
    set_tests_properties(${testName} PROPERTIES FIXTURES_REQUIRED TestStage)
  endif()
endforeach()
