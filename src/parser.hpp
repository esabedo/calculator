#pragma once

#include "lexer.hpp"
#include "ast/node.hpp"
#include "ast/number.hpp"
#include "ast/binary_op.hpp"
#include "ast/unary_op.hpp"
#include "ast/func_call.hpp"
#include "error.hpp"
#include <memory>
#include <vector>
#include <stack>
#include <unordered_map>

namespace calc {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::unique_ptr<Node> parse();
    
private:
    std::vector<Token> tokens_;
    size_t pos_;
    
    Token& current();
    Token& peek(size_t offset = 0);
    void advance();
    bool match(TokenType type);
    
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseTerm();
    std::unique_ptr<Node> parseFactor();
    std::unique_ptr<Node> parsePower();
    std::unique_ptr<Node> parseUnary();
    std::unique_ptr<Node> parsePrimary();
    
    int getPrecedence(TokenType type);
    bool isRightAssociative(TokenType type);
};

} // namespace calc
