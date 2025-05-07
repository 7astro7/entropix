cmake_minimum_required(VERSION 3.14)
project(entropix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb3 -O0")

# Copy test data directory into build dir
file(COPY ${CMAKE_SOURCE_DIR}/test/data DESTINATION ${CMAKE_BINARY_DIR}/test/data)

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

include(FetchContent)

# nlohmann/json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ------------------------------------------------------------------------------
# Shared Library
# ------------------------------------------------------------------------------

include_directories(src)

add_library(entropix
    src/file_reader.cpp
    src/entropy_calculator.cpp
    src/block_entropy_scanner.cpp
    src/utils.cpp
)

target_link_libraries(entropix
    PRIVATE
    nlohmann_json::nlohmann_json
)

# ------------------------------------------------------------------------------
# CLI Executable
# ------------------------------------------------------------------------------

add_executable(entropix_cli main.cpp)

target_link_libraries(entropix_cli
    PRIVATE
    entropix
    nlohmann_json::nlohmann_json
)

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

add_executable(runTests
    test/test_file_reader.cpp
    test/test_entropy_calculator.cpp
    test/test_block_entropy_scanner.cpp
    test/test_utils.cpp
)

target_link_libraries(runTests
    PRIVATE
    gtest_main
    entropix
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(runTests)

# ------------------------------------------------------------------------------
# Compile-Time Definitions
# ------------------------------------------------------------------------------

add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")
