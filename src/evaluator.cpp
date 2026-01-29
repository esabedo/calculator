#include "evaluator.hpp"

namespace calc {

double Evaluator::evaluate(const std::unique_ptr<Node>& root) {
    if (!root) {
        return 0.0;
    }
    return root->evaluate();
}

} // namespace calc
