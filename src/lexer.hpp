#pragma once

#include <string>
#include <vector>
#include <variant>
#include <cctype>
#include <stdexcept>

namespace calc {

enum class TokenType {
    Number,
    Identifier,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    Power,
    LParen,
    RParen,
    Comma,
    // Битовые операции
    BitwiseAnd,    // AND
    BitwiseOr,     // OR
    BitwiseXor,    // XOR
    BitwiseNot,    // NOT
    LeftShift,     // <<
    RightShift,    // >>
    End
};


struct Token {
    TokenType type;
    std::variant<double, std::string> value;
    
    Token(TokenType t) : type(t) {}
    Token(TokenType t, double v) : type(t), value(v) {}
    Token(TokenType t, std::string v) : type(t), value(std::move(v)) {}
};

class Lexer {
public:
    explicit Lexer(std::string input);
    std::vector<Token> tokenize();
    
private:
    std::string input_;
    size_t pos_;
    
    void skipWhitespace();
    Token parseNumber();
    Token parseIdentifier();
    char peek() const;
    char get();
};

} // namespace calc
