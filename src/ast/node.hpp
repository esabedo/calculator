#pragma once

namespace calc {

class Node {
public:
    virtual ~Node() = default;
    virtual double evaluate() const = 0;
};

} // namespace calc
