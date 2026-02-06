#include "parser.hpp"
#include <stdexcept>

namespace calc {

Parser::Parser(std::vector<Token> tokens) 
    : tokens_(std::move(tokens)), pos_(0) {}

Token& Parser::current() {
    if (pos_ >= tokens_.size()) {
        throw ParseError("Unexpected end of input");
    }
    return tokens_[pos_];
}

Token& Parser::peek(size_t offset) {
    if (pos_ + offset >= tokens_.size()) {
        throw ParseError("Unexpected end of input");
    }
    return tokens_[pos_ + offset];
}

void Parser::advance() {
    if (pos_ < tokens_.size()) {
        ++pos_;
    }
}

bool Parser::match(TokenType type) {
    if (current().type == type) {
        advance();
        return true;
    }
    return false;
}

int Parser::getPrecedence(TokenType type) {
    switch (type) {
        case TokenType::Plus:
        case TokenType::Minus:
            return 1;
        case TokenType::Multiply:
        case TokenType::Divide:
        case TokenType::Modulo:
            return 2;
        case TokenType::Power:
            return 3;
        default:
            return 0;
    }
}

bool Parser::isRightAssociative(TokenType type) {
    return type == TokenType::Power;
}

std::unique_ptr<Node> Parser::parse() {
    auto result = parseExpression();
    if (current().type != TokenType::End) {
        throw ParseError("Unexpected token after expression");
    }
    return result;
}

std::unique_ptr<Node> Parser::parseExpression() {
    return parseBitwiseOr();
}

// Битовое ИЛИ (самый низкий приоритет)
std::unique_ptr<Node> Parser::parseBitwiseOr() {
    auto left = parseBitwiseXor();
    
    while (current().type == TokenType::BitwiseOr) {
        advance();
        auto right = parseBitwiseXor();
        left = std::make_unique<BinaryOpNode>(BinaryOp::BitwiseOr, std::move(left), std::move(right));
    }
    
    return left;
}

// Битовое исключающее ИЛИ
std::unique_ptr<Node> Parser::parseBitwiseXor() {
    auto left = parseBitwiseAnd();
    
    while (current().type == TokenType::BitwiseXor) {
        advance();
        auto right = parseBitwiseAnd();
        left = std::make_unique<BinaryOpNode>(BinaryOp::BitwiseXor, std::move(left), std::move(right));
    }
    
    return left;
}

// Битовое И
std::unique_ptr<Node> Parser::parseBitwiseAnd() {
    auto left = parseShift();
    
    while (current().type == TokenType::BitwiseAnd) {
        advance();
        auto right = parseShift();
        left = std::make_unique<BinaryOpNode>(BinaryOp::BitwiseAnd, std::move(left), std::move(right));
    }
    
    return left;
}

// Битовые сдвиги
std::unique_ptr<Node> Parser::parseShift() {
    auto left = parseTerm();
    
    while (current().type == TokenType::LeftShift || current().type == TokenType::RightShift) {
        TokenType op = current().type;
        advance();
        auto right = parseTerm();
        
        BinaryOp binOp = (op == TokenType::LeftShift) ? BinaryOp::LeftShift : BinaryOp::RightShift;
        left = std::make_unique<BinaryOpNode>(binOp, std::move(left), std::move(right));
    }
    
    return left;
}

// Сложение и вычитание
std::unique_ptr<Node> Parser::parseTerm() {
    auto left = parseFactor();
    
    while (current().type == TokenType::Plus || current().type == TokenType::Minus) {
        TokenType op = current().type;
        advance();
        auto right = parseFactor();
        
        BinaryOp binOp = (op == TokenType::Plus) ? BinaryOp::Add : BinaryOp::Subtract;
        left = std::make_unique<BinaryOpNode>(binOp, std::move(left), std::move(right));
    }
    
    return left;
}

// Умножение, деление, остаток
std::unique_ptr<Node> Parser::parseFactor() {
    auto left = parsePower();
    
    while (current().type == TokenType::Multiply || 
           current().type == TokenType::Divide || 
           current().type == TokenType::Modulo) {
        TokenType op = current().type;
        advance();
        auto right = parsePower();
        
        BinaryOp binOp;
        if (op == TokenType::Multiply) {
            binOp = BinaryOp::Multiply;
        } else if (op == TokenType::Divide) {
            binOp = BinaryOp::Divide;
        } else {
            binOp = BinaryOp::Modulo;
        }
        
        left = std::make_unique<BinaryOpNode>(binOp, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<Node> Parser::parsePower() {
    auto left = parseUnary();
    
    if (current().type == TokenType::Power) {
        advance();
        auto right = parsePower(); // Right associative
        left = std::make_unique<BinaryOpNode>(BinaryOp::Power, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<Node> Parser::parseUnary() {
    if (current().type == TokenType::Plus) {
        advance();
        auto operand = parseUnary();
        return std::make_unique<UnaryOpNode>(UnaryOp::Plus, std::move(operand));
    }
    
    if (current().type == TokenType::Minus) {
        advance();
        auto operand = parseUnary();
        return std::make_unique<UnaryOpNode>(UnaryOp::Minus, std::move(operand));
    }
    
    if (current().type == TokenType::BitwiseNot) {
        advance();
        auto operand = parseUnary();
        return std::make_unique<UnaryOpNode>(UnaryOp::BitwiseNot, std::move(operand));
    }
    
    return parsePrimary();
}

std::unique_ptr<Node> Parser::parsePrimary() {
    Token& tok = current();
    
    if (tok.type == TokenType::Number) {
        double value = std::get<double>(tok.value);
        advance();
        return std::make_unique<NumberNode>(value);
    }
    
    if (tok.type == TokenType::Identifier) {
        std::string name = std::get<std::string>(tok.value);
        advance();
        
        if (match(TokenType::LParen)) {
            auto arg = parseExpression();
            // Разрешаем опускать закрывающую скобку, если достигнут конец ввода
            if (!match(TokenType::RParen) && current().type != TokenType::End) {
                throw ParseError("Expected ')' after function argument");
            }
            return std::make_unique<FuncCallNode>(name, std::move(arg));
        }
        
        throw ParseError("Unknown identifier: " + name);
    }
    
    if (match(TokenType::LParen)) {
        auto expr = parseExpression();
        // Разрешаем опускать закрывающую скобку, если достигнут конец ввода
        if (!match(TokenType::RParen) && current().type != TokenType::End) {
            throw ParseError("Expected ')'");
        }
        return expr;
    }
    
    throw ParseError("Unexpected token");
}

} // namespace calc
