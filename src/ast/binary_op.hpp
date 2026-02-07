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
    Power,
    // Битовые операции
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    LeftShift,
    RightShift
};

class BinaryOpNode : public Node {
public:
    BinaryOpNode(BinaryOp op, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : op_(op), left_(std::move(left)), right_(std::move(right)) {}
    
    double evaluate() const override {
        if (!left_ || !right_) {
            throw EvalError("Invalid operands: null pointer");
        }
        
        double left_val = left_->evaluate();
        double right_val = right_->evaluate();
        
        // Проверка на NaN и Infinity
        if (std::isnan(left_val) || std::isnan(right_val)) {
            throw EvalError("Invalid operand: NaN");
        }
        if (std::isinf(left_val) || std::isinf(right_val)) {
            throw EvalError("Invalid operand: Infinity");
        }
        
        double result = 0.0;
        
        switch (op_) {
            case BinaryOp::Add:
                result = left_val + right_val;
                if (std::isinf(result)) {
                    throw EvalError("Overflow in addition");
                }
                return result;
                
            case BinaryOp::Subtract:
                result = left_val - right_val;
                if (std::isinf(result)) {
                    throw EvalError("Overflow in subtraction");
                }
                return result;
                
            case BinaryOp::Multiply:
                result = left_val * right_val;
                if (std::isinf(result)) {
                    throw EvalError("Overflow in multiplication");
                }
                return result;
                
            case BinaryOp::Divide:
                if (std::abs(right_val) < 1e-15) {
                    throw EvalError("Division by zero");
                }
                result = left_val / right_val;
                if (std::isinf(result)) {
                    throw EvalError("Overflow in division");
                }
                return result;
                
            case BinaryOp::Modulo:
                if (std::abs(right_val) < 1e-15) {
                    throw EvalError("Modulo by zero");
                }
                result = std::fmod(left_val, right_val);
                return result;
                
            case BinaryOp::Power:
                // Проверка на потенциальное переполнение
                if (left_val == 0.0 && right_val < 0.0) {
                    throw EvalError("Zero to negative power");
                }
                if (left_val < 0.0 && std::floor(right_val) != right_val) {
                    throw EvalError("Negative base with non-integer exponent");
                }
                result = std::pow(left_val, right_val);
                if (std::isinf(result)) {
                    throw EvalError("Overflow in power operation");
                }
                if (std::isnan(result)) {
                    throw EvalError("Invalid power operation result");
                }
                return result;
                
            // Битовые операции
            case BinaryOp::BitwiseAnd:
            case BinaryOp::BitwiseOr:
            case BinaryOp::BitwiseXor:
            case BinaryOp::LeftShift:
            case BinaryOp::RightShift: {
                // Проверка диапазона для битовых операций
                constexpr double MAX_INT64 = static_cast<double>(INT64_MAX);
                constexpr double MIN_INT64 = static_cast<double>(INT64_MIN);
                
                if (left_val > MAX_INT64 || left_val < MIN_INT64) {
                    throw EvalError("Left operand out of int64 range for bitwise operation");
                }
                if (right_val > MAX_INT64 || right_val < MIN_INT64) {
                    throw EvalError("Right operand out of int64 range for bitwise operation");
                }
                
                int64_t left_int = static_cast<int64_t>(left_val);
                int64_t right_int = static_cast<int64_t>(right_val);
                
                // Дополнительные проверки для сдвигов
                if (op_ == BinaryOp::LeftShift || op_ == BinaryOp::RightShift) {
                    if (right_int < 0) {
                        throw EvalError("Negative shift count");
                    }
                    if (right_int >= 64) {
                        throw EvalError("Shift count too large (>= 64)");
                    }
                }
                
                int64_t int_result = 0;
                switch (op_) {
                    case BinaryOp::BitwiseAnd:
                        int_result = left_int & right_int;
                        break;
                    case BinaryOp::BitwiseOr:
                        int_result = left_int | right_int;
                        break;
                    case BinaryOp::BitwiseXor:
                        int_result = left_int ^ right_int;
                        break;
                    case BinaryOp::LeftShift:
                        int_result = left_int << right_int;
                        break;
                    case BinaryOp::RightShift:
                        int_result = left_int >> right_int;
                        break;
                    default:
                        break;
                }
                return static_cast<double>(int_result);
            }
        }
        throw EvalError("Unknown binary operator");
    }
    
private:
    BinaryOp op_;
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;
};

} // namespace calc
