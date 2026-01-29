cmake_minimum_required(VERSION 3.12)

project(Calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
    src/main.cpp
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

add_executable(calc ${SOURCES} ${HEADERS})

target_include_directories(calc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
