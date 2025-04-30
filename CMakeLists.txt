cmake_minimum_required(VERSION 3.14)
project(entropix)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)  

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(entropix
    src/file_reader.cpp
    src/entropy_calculator.cpp
)

add_executable(runTests
    test/test_file_reader.cpp
    test/test_entropy_calculator.cpp   
)

target_link_libraries(runTests
    gtest_main
    entropix
)

include(GoogleTest)
gtest_discover_tests(runTests)

add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")


