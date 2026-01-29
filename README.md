# Calculator

A console calculator written in C++17 that supports complex expressions with parentheses and mathematical functions.

## Features

- **Basic arithmetic operations**: addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), power (`^`)
- **Parentheses**: support for nested expressions with proper precedence
- **Mathematical functions**: `sin`, `cos`, `tan`, `log`, `exp`, `sqrt`
- **Unary operators**: positive (`+`) and negative (`-`) signs
- **Decimal numbers**: support for floating-point numbers
- **Error handling**: clear error messages for invalid expressions, division by zero, mismatched parentheses

## Usage

### Command Line Arguments

```bash
# Calculate expression from command line argument
./calc "2 + 3 * 4"
./calc "sin(1.57079632679)"
./calc "(10 - 5) / (2 + 3)"

# Show help
./calc --help
./calc -h

# Read from stdin
echo "3 + 4 * 2" | ./calc
```

### Examples

```bash
$ ./calc "2 + 3 * 4"
14

$ ./calc "sin(1.57079632679)"
1

$ ./calc "sqrt(16) + 5"
9

$ ./calc "2 ^ 3 ^ 2"
512
```

## Building

### Requirements

- CMake 3.12 or higher
- C++17 compatible compiler
- Google Test (automatically downloaded if not found)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests

Tests are built and run automatically during the build process:

```bash
cmake ..
make
```

To run tests manually:

```bash
./calc_tests
```

## Architecture

The calculator consists of three main components:

1. **Lexer** (`src/lexer.cpp`): Tokenizes the input string into tokens (numbers, operators, functions, parentheses)
2. **Parser** (`src/parser.cpp`): Parses tokens into an Abstract Syntax Tree (AST) using recursive descent with operator precedence
3. **Evaluator** (`src/evaluator.cpp`): Evaluates the AST and returns the result

### AST Nodes

- `NumberNode`: Represents numeric literals
- `BinaryOpNode`: Binary operations (+, -, *, /, ^)
- `UnaryOpNode`: Unary operations (+, -)
- `FunctionNode`: Function calls (sin, cos, etc.)

## Project Structure

```
calculator/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── .gitignore              # Git ignore rules
├── src/
│   ├── main.cpp            # Entry point
│   ├── lexer.cpp/hpp       # Lexical analyzer
│   ├── parser.cpp/hpp      # Syntax parser
│   ├── evaluator.cpp/hpp   # Expression evaluator
│   ├── error.hpp           # Error handling
│   └── ast/                # AST node definitions
│       ├── node.hpp
│       ├── number.hpp
│       ├── binary_op.hpp
│       ├── unary_op.hpp
│       └── func_call.hpp
└── tests/
    └── test_calculator.cpp # Unit tests
```

## License

MIT License
