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
    auto left = parseTerm();
    
    while (current().type == TokenType::Plus || current().type == TokenType::Minus) {
        TokenType op = current().type;
        advance();
        auto right = parseTerm();
        
        BinaryOp binOp = (op == TokenType::Plus) ? BinaryOp::Add : BinaryOp::Subtract;
        left = std::make_unique<BinaryOpNode>(binOp, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<Node> Parser::parseTerm() {
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
            if (!match(TokenType::RParen)) {
                throw ParseError("Expected ')' after function argument");
            }
            return std::make_unique<FuncCallNode>(name, std::move(arg));
        }
        
        throw ParseError("Unknown identifier: " + name);
    }
    
    if (match(TokenType::LParen)) {
        auto expr = parseExpression();
        if (!match(TokenType::RParen)) {
            throw ParseError("Expected ')'");
        }
        return expr;
    }
    
    throw ParseError("Unexpected token");
}

} // namespace calc
