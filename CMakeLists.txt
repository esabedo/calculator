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

# Console executable
add_executable(calc ${CORE_SOURCES} src/main.cpp ${HEADERS})
target_include_directories(calc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Qt GUI version
option(BUILD_GUI "Build GUI version with Qt" ON)
if(BUILD_GUI)
    # Find Qt5
    find_package(Qt5 COMPONENTS Widgets QUIET)
    
    if(Qt5_FOUND)
        message(STATUS "Qt5 found, building GUI version")
        
        # Enable Qt features
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_AUTOUIC ON)
        
        # GUI sources
        set(GUI_SOURCES
            src/gui/MainWindow.cpp
            src/gui/CalculatorWidget.cpp
            src/gui/CalculatorButton.cpp
        )
        
        set(GUI_HEADERS
            src/gui/MainWindow.hpp
            src/gui/CalculatorWidget.hpp
            src/gui/CalculatorButton.hpp
        )
        
        # GUI executable
        add_executable(calc-gui 
            ${CORE_SOURCES} 
            ${GUI_SOURCES}
            src/main_gui.cpp
            ${HEADERS}
            ${GUI_HEADERS}
        )
        
        target_include_directories(calc-gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_link_libraries(calc-gui Qt5::Widgets)
        
        # Copy styles.qss to build directory
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/styles.qss
            ${CMAKE_CURRENT_BINARY_DIR}/styles.qss
            COPYONLY
        )
    else()
        message(WARNING "Qt5 not found, GUI version will not be built")
    endif()
endif()

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
    
    # Custom target to run tests as part of the default build
    add_custom_target(run_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS calc_tests
        COMMENT "Running tests"
    )
endif()
