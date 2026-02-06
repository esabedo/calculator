#include "lexer.hpp"
#include "error.hpp"
#include <cctype>

namespace calc {

Lexer::Lexer(std::string input) : input_(std::move(input)), pos_(0) {}

char Lexer::peek() const {
    if (pos_ >= input_.size()) return '\0';
    return input_[pos_];
}

char Lexer::get() {
    if (pos_ >= input_.size()) return '\0';
    return input_[pos_++];
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek()))) {
        get();
    }
}

Token Lexer::parseNumber() {
    std::string numStr;
    bool hasDot = false;
    
    while (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.') {
        if (peek() == '.') {
            if (hasDot) break;
            hasDot = true;
        }
        numStr += get();
    }
    
    if (numStr.empty() || numStr == ".") {
        throw ParseError("Invalid number format");
    }
    
    return Token(TokenType::Number, std::stod(numStr));
}

Token Lexer::parseIdentifier() {
    std::string id;
    
    while (std::isalpha(static_cast<unsigned char>(peek())) || 
           std::isdigit(static_cast<unsigned char>(peek())) || 
           peek() == '_') {
        id += get();
    }
    
    // Проверка на математические константы
    if (id == "pi" || id == "PI") {
        return Token(TokenType::Number, 3.14159265358979323846);
    } else if (id == "e" || id == "E") {
        return Token(TokenType::Number, 2.71828182845904523536);
    }
    
    return Token(TokenType::Identifier, id);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (true) {
        skipWhitespace();
        char c = peek();
        
        if (c == '\0') {
            tokens.emplace_back(TokenType::End);
            break;
        }
        
        switch (c) {
            case '+':
                get();
                tokens.emplace_back(TokenType::Plus);
                break;
            case '-':
                get();
                tokens.emplace_back(TokenType::Minus);
                break;
            case '*':
                get();
                if (peek() == '*') {
                    get();
                    tokens.emplace_back(TokenType::Power);
                } else {
                    tokens.emplace_back(TokenType::Multiply);
                }
                break;
            case '/':
                get();
                tokens.emplace_back(TokenType::Divide);
                break;
            case '%':
                get();
                tokens.emplace_back(TokenType::Modulo);
                break;
            case '^':
                get();
                tokens.emplace_back(TokenType::Power);
                break;
            case '(':
                get();
                tokens.emplace_back(TokenType::LParen);
                break;
            case ')':
                get();
                tokens.emplace_back(TokenType::RParen);
                break;
            case ',':
                get();
                tokens.emplace_back(TokenType::Comma);
                break;
            default:
                if (std::isdigit(static_cast<unsigned char>(c))) {
                    tokens.push_back(parseNumber());
                } else if (std::isalpha(static_cast<unsigned char>(c))) {
                    tokens.push_back(parseIdentifier());
                } else {
                    throw ParseError(std::string("Unknown character: ") + c);
                }
        }
    }
    
    return tokens;
}

} // namespace calc
