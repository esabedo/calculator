#pragma once

#include "node.hpp"
#include "../error.hpp"
#include <memory>
#include <cmath>

namespace calc {

enum class BinaryOp {
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Power
};

class BinaryOpNode : public Node {
public:
    BinaryOpNode(BinaryOp op, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : op_(op), left_(std::move(left)), right_(std::move(right)) {}
    
    double evaluate() const override {
        double left_val = left_->evaluate();
        double right_val = right_->evaluate();
        
        switch (op_) {
            case BinaryOp::Add:
                return left_val + right_val;
            case BinaryOp::Subtract:
                return left_val - right_val;
            case BinaryOp::Multiply:
                return left_val * right_val;
            case BinaryOp::Divide:
                if (right_val == 0.0) {
                    throw EvalError("Деление на ноль");
                }
                return left_val / right_val;
            case BinaryOp::Modulo:
                if (right_val == 0.0) {
                    throw EvalError("Деление на ноль при взятии остатка");
                }
                return std::fmod(left_val, right_val);
            case BinaryOp::Power:
                return std::pow(left_val, right_val);
        }
        throw EvalError("Unknown binary operator");
    }
    
private:
    BinaryOp op_;
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;
};

} // namespace calc
