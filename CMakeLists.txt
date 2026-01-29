cmake_minimum_required(VERSION 3.12)

project(Calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core library sources (without main.cpp)
set(CORE_SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
)

set(HEADERS
    src/lexer.hpp
    src/parser.hpp
    src/evaluator.hpp
    src/error.hpp
    src/ast/node.hpp
    src/ast/number.hpp
    src/ast/binary_op.hpp
    src/ast/unary_op.hpp
    src/ast/func_call.hpp
)

# Main executable
add_executable(calc ${CORE_SOURCES} src/main.cpp ${HEADERS})
target_include_directories(calc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
        )
        FetchContent_MakeAvailable(googletest)
    endif()

    enable_testing()
    add_executable(calc_tests ${CORE_SOURCES} tests/test_calculator.cpp ${HEADERS})
    target_include_directories(calc_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(calc_tests GTest::gtest_main)
    
    include(GoogleTest)
    gtest_discover_tests(calc_tests)
endif()
