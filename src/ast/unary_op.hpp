#pragma once

#include "node.hpp"
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
        double val = operand_->evaluate();
        
        switch (op_) {
            case UnaryOp::Plus:
                return +val;
            case UnaryOp::Minus:
                return -val;
            case UnaryOp::BitwiseNot:
                return static_cast<double>(~static_cast<int64_t>(val));
        }
        return val;
    }
    
private:
    UnaryOp op_;
    std::unique_ptr<Node> operand_;
};

} // namespace calc
