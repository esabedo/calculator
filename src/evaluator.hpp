#pragma once

#include "ast/node.hpp"
#include <memory>

namespace calc {

class Evaluator {
public:
    double evaluate(const std::unique_ptr<Node>& root);
};

} // namespace calc
