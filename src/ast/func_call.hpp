#pragma once

#include "node.hpp"
#include "../error.hpp"
#include <memory>
#include <cmath>
#include <string>
#include <unordered_map>
#include <functional>

namespace calc {

class FuncCallNode : public Node {
public:
    FuncCallNode(const std::string& name, std::unique_ptr<Node> arg)
        : name_(name), arg_(std::move(arg)) {}
    
    double evaluate() const override {
        if (!arg_) {
            throw EvalError("Invalid function argument: null pointer");
        }
        
        double val = arg_->evaluate();
        
        // Проверка на NaN и Infinity во входных данных
        if (std::isnan(val)) {
            throw EvalError("Invalid function argument: NaN");
        }
        if (std::isinf(val)) {
            throw EvalError("Invalid function argument: Infinity");
        }
        
        static const std::unordered_map<std::string, std::function<double(double)>> funcs = {
            // Базовые тригонометрические функции
            {"sin", [](double x) { 
                double result = std::sin(x);
                if (std::isnan(result)) throw EvalError("sin: invalid result");
                return result;
            }},
            {"cos", [](double x) { 
                double result = std::cos(x);
                if (std::isnan(result)) throw EvalError("cos: invalid result");
                return result;
            }},
            {"tan", [](double x) { 
                double result = std::tan(x);
                if (std::isnan(result) || std::isinf(result)) {
                    throw EvalError("tan: result is undefined or infinite");
                }
                return result;
            }},
            
            // Обратные тригонометрические функции
            {"asin", [](double x) { 
                if (x < -1.0 || x > 1.0) {
                    throw EvalError("asin: argument must be in range [-1, 1]");
                }
                return std::asin(x); 
            }},
            {"acos", [](double x) { 
                if (x < -1.0 || x > 1.0) {
                    throw EvalError("acos: argument must be in range [-1, 1]");
                }
                return std::acos(x); 
            }},
            {"atan", [](double x) { return std::atan(x); }},
            
            // Гиперболические функции
            {"sinh", [](double x) { 
                double result = std::sinh(x);
                if (std::isinf(result)) throw EvalError("sinh: overflow");
                return result;
            }},
            {"cosh", [](double x) { 
                double result = std::cosh(x);
                if (std::isinf(result)) throw EvalError("cosh: overflow");
                return result;
            }},
            {"tanh", [](double x) { return std::tanh(x); }},
            
            // Логарифмические функции
            {"log", [](double x) { 
                if (x <= 0.0) throw EvalError("log: argument must be positive");
                double result = std::log(x);
                if (std::isinf(result)) throw EvalError("log: result is infinite");
                return result;
            }},
            {"ln", [](double x) { 
                if (x <= 0.0) throw EvalError("ln: argument must be positive");
                double result = std::log(x);
                if (std::isinf(result)) throw EvalError("ln: result is infinite");
                return result;
            }},
            {"log10", [](double x) { 
                if (x <= 0.0) throw EvalError("log10: argument must be positive");
                double result = std::log10(x);
                if (std::isinf(result)) throw EvalError("log10: result is infinite");
                return result;
            }},
            
            // Экспонента и корень
            {"exp", [](double x) { 
                if (x > 709.0) throw EvalError("exp: argument too large, would overflow");
                double result = std::exp(x);
                if (std::isinf(result)) throw EvalError("exp: overflow");
                return result;
            }},
            {"sqrt", [](double x) { 
                if (x < 0.0) throw EvalError("sqrt: argument must be non-negative");
                return std::sqrt(x); 
            }},
            
            // Дополнительные математические функции
            {"abs", [](double x) { return std::abs(x); }},
            {"ceil", [](double x) { return std::ceil(x); }},
            {"floor", [](double x) { return std::floor(x); }},
            {"round", [](double x) { return std::round(x); }},
            
            // Факториал (для целых чисел)
            {"factorial", [](double x) {
                if (x < 0.0) {
                    throw EvalError("factorial: argument must be non-negative");
                }
                if (x != std::floor(x)) {
                    throw EvalError("factorial: argument must be an integer");
                }
                if (x > 170.0) {
                    throw EvalError("factorial: argument too large (max 170)");
                }
                
                double result = 1.0;
                for (int i = 2; i <= static_cast<int>(x); ++i) {
                    result *= i;
                    if (std::isinf(result)) {
                        throw EvalError("factorial: overflow during calculation");
                    }
                }
                return result;
            }}
        };
        
        auto it = funcs.find(name_);
        if (it == funcs.end()) {
            throw EvalError("Unknown function: " + name_);
        }
        
        double result = it->second(val);
        
        // Финальная проверка результата
        if (std::isnan(result)) {
            throw EvalError(name_ + ": result is NaN");
        }
        
        return result;
    }
    
private:
    std::string name_;
    std::unique_ptr<Node> arg_;
};

} // namespace calc
