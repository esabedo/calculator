# Calculator

A command-line calculator that evaluates mathematical expressions with support for arithmetic operations, functions, and parentheses.

## Features

- **Arithmetic operations**: addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), power (`^`)
- **Mathematical functions**: `sin`, `cos`, `tan`, `log`, `exp`, `sqrt`
- **Parentheses** for grouping expressions
- **Unary operators**: unary plus (`+`) and unary minus (`-`)
- **Decimal numbers** support
- **Whitespace handling**: spaces are ignored
- **Operator precedence**: power is right-associative, others follow standard precedence
- **Error handling**: clear error messages for syntax errors, division by zero, unknown functions

## Usage

### Command-line arguments

```bash
./calc "2 + 3 * 4"
./calc "sin(pi/2)"
./calc "sqrt(16) + log(10)"
./calc --help    # Show help message
./calc -h        # Show help message
```

### Standard input

```bash
echo "2 + 3 * 4" | ./calc
./calc           # Then type expression and press Enter
```

## Examples

```bash
$ ./calc "2 + 3 * 4"
14

$ ./calc "(2 + 3) * 4"
20

$ ./calc "2 ^ 3 ^ 2"
512

$ ./calc "sin(1.57079632679)"
1

$ ./calc "sqrt(16) + log(2.71828182846)"
3
```

## Building

### Requirements

- CMake 3.12 or higher
- C++17 compatible compiler
- Google Test (automatically fetched if not found)

### Build steps

```bash
# Create build directory
mkdir build
cd build

# Configure with tests enabled (default)
cmake ..

# Build (this will also build and run tests)
make

# Or use CMake directly
cmake --build .
```

### Build options

- `BUILD_TESTS` - Build tests (default: ON)

```bash
cmake -DBUILD_TESTS=OFF ..  # Disable tests
cmake -DBUILD_TESTS=ON ..   # Enable tests (default)
```

## Running Tests

Tests are automatically built and run when using `make` or `cmake --build .`.

To run tests manually:

```bash
cd build
./calc_tests
```

Or using CTest:

```bash
cd build
ctest
```

## Project Structure

```
.
├── CMakeLists.txt          # CMake configuration
├── src/
│   ├── main.cpp            # CLI entry point
│   ├── lexer.cpp/hpp       # Tokenizer
│   ├── parser.cpp/hpp      # Expression parser (AST builder)
│   ├── evaluator.cpp/hpp   # AST evaluator
│   ├── error.hpp           # Exception types
│   └── ast/                # AST node types
│       ├── node.hpp
│       ├── number.hpp
│       ├── binary_op.hpp
│       ├── unary_op.hpp
│       └── func_call.hpp
└── tests/
    └── test_calculator.cpp # Unit tests
```

## Architecture

The calculator uses a three-stage pipeline:

1. **Lexer** - Converts input string into tokens (numbers, operators, functions, parentheses)
2. **Parser** - Builds an Abstract Syntax Tree (AST) from tokens using recursive descent
3. **Evaluator** - Traverses the AST and computes the result

This design allows for clean separation of concerns and easy extensibility.
