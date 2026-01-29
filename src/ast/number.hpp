#pragma once

#include "node.hpp"

namespace calc {

class NumberNode : public Node {
public:
    explicit NumberNode(double value) : value_(value) {}
    
    double evaluate() const override {
        return value_;
    }
    
private:
    double value_;
};

} // namespace calc
