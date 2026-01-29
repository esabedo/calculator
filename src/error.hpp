#pragma once

#include <stdexcept>
#include <string>

namespace calc {

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message)
        : std::runtime_error(message) {}
};

class EvalError : public std::runtime_error {
public:
    explicit EvalError(const std::string& message)
        : std::runtime_error(message) {}
};

} // namespace calc
