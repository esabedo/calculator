#include "lexer.hpp"
#include "error.hpp"
#include <cctype>
#include <cmath>

namespace calc {

Lexer::Lexer(std::string input) : input_(std::move(input)), pos_(0) {
    // Защита от слишком длинных входных строк
    if (input_.size() > 10000) {
        throw ParseError("Input string too long (max 10000 characters)");
    }
}

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
    bool hasE = false;
    
    // Защита от слишком длинных чисел
    constexpr size_t MAX_NUMBER_LENGTH = 100;
    
    while (pos_ < input_.size()) {
        char c = peek();
        
        if (std::isdigit(static_cast<unsigned char>(c))) {
            numStr += get();
        } else if (c == '.') {
            if (hasDot || hasE) break;  // Только одна точка и не после E
            hasDot = true;
            numStr += get();
        } else if (c == 'e' || c == 'E') {
            if (hasE) break;  // Только один экспоненциальный символ
            hasE = true;
            numStr += get();
            // Проверка на знак после E
            if (peek() == '+' || peek() == '-') {
                numStr += get();
            }
        } else {
            break;
        }
        
        if (numStr.size() > MAX_NUMBER_LENGTH) {
            throw ParseError("Number too long (max 100 characters)");
        }
    }
    
    if (numStr.empty() || numStr == "." || numStr == "e" || numStr == "E") {
        throw ParseError("Invalid number format");
    }
    
    // Проверка на корректное окончание
    char last = numStr.back();
    if (last == '.' || last == 'e' || last == 'E' || last == '+' || last == '-') {
        throw ParseError("Invalid number format: incomplete");
    }
    
    try {
        double value = std::stod(numStr);
        
        // Проверка на переполнение
        if (std::isinf(value)) {
            throw ParseError("Number overflow: value too large");
        }
        if (std::isnan(value)) {
            throw ParseError("Invalid number: NaN");
        }
        
        return Token(TokenType::Number, value);
    } catch (const std::out_of_range&) {
        throw ParseError("Number out of range");
    } catch (const std::invalid_argument&) {
        throw ParseError("Invalid number format");
    }
}

Token Lexer::parseIdentifier() {
    std::string id;
    constexpr size_t MAX_IDENTIFIER_LENGTH = 100;
    
    while (std::isalpha(static_cast<unsigned char>(peek())) || 
           std::isdigit(static_cast<unsigned char>(peek())) || 
           peek() == '_') {
        id += get();
        
        if (id.size() > MAX_IDENTIFIER_LENGTH) {
            throw ParseError("Identifier too long (max 100 characters)");
        }
    }
    
    if (id.empty()) {
        throw ParseError("Empty identifier");
    }
    
    // Проверка на математические константы
    if (id == "pi" || id == "PI") {
        return Token(TokenType::Number, 3.14159265358979323846);
    } else if (id == "e" || id == "E") {
        return Token(TokenType::Number, 2.71828182845904523536);
    }
    
    // Проверка на битовые операции
    if (id == "AND") {
        return Token(TokenType::BitwiseAnd);
    } else if (id == "OR") {
        return Token(TokenType::BitwiseOr);
    } else if (id == "XOR") {
        return Token(TokenType::BitwiseXor);
    } else if (id == "NOT") {
        return Token(TokenType::BitwiseNot);
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
            case '<':
                get();
                if (peek() == '<') {
                    get();
                    tokens.emplace_back(TokenType::LeftShift);
                } else {
                    throw ParseError("Unexpected character: <");
                }
                break;
            case '>':
                get();
                if (peek() == '>') {
                    get();
                    tokens.emplace_back(TokenType::RightShift);
                } else {
                    throw ParseError("Unexpected character: >");
                }
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
