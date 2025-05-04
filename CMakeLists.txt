cmake_minimum_required(VERSION 3.14)
project(entropix)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb3 -O0")

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
    src/block_entropy_scanner.cpp  
)

add_executable(runTests
    test/test_file_reader.cpp
    test/test_entropy_calculator.cpp   
    test/test_block_entropy_scanner.cpp
)

add_executable(entropix_cli main.cpp)

target_link_libraries(entropix_cli
    entropix
)

target_link_libraries(runTests
    gtest_main
    entropix
)

include(GoogleTest)
gtest_discover_tests(runTests)

add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")


