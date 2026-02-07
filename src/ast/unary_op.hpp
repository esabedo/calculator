#pragma once

#include "node.hpp"
#include "../error.hpp"
#include <memory>
#include <cmath>

namespace calc {

enum class UnaryOp {
    Plus,
    Minus,
    BitwiseNot  // Битовое НЕ
};


class UnaryOpNode : public Node {
public:
    UnaryOpNode(UnaryOp op, std::unique_ptr<Node> operand)
        : op_(op), operand_(std::move(operand)) {}
    
    double evaluate() const override {
        if (!operand_) {
            throw EvalError("Invalid operand: null pointer");
        }
        
        double val = operand_->evaluate();
        
        // Проверка на NaN и Infinity
        if (std::isnan(val)) {
            throw EvalError("Invalid operand: NaN");
        }
        if (std::isinf(val)) {
            throw EvalError("Invalid operand: Infinity");
        }
        
        switch (op_) {
            case UnaryOp::Plus:
                return val;
                
            case UnaryOp::Minus:
                return -val;
                
            case UnaryOp::BitwiseNot: {
                // Проверка диапазона для битовой операции
                constexpr double MAX_INT64 = static_cast<double>(INT64_MAX);
                constexpr double MIN_INT64 = static_cast<double>(INT64_MIN);
                
                if (val > MAX_INT64 || val < MIN_INT64) {
                    throw EvalError("Operand out of int64 range for bitwise NOT");
                }
                
                int64_t int_val = static_cast<int64_t>(val);
                return static_cast<double>(~int_val);
            }
        }
        throw EvalError("Unknown unary operator");
    }
    
private:
    UnaryOp op_;
    std::unique_ptr<Node> operand_;
};

} // namespace calc
