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
        double val = arg_->evaluate();
        
        static const std::unordered_map<std::string, std::function<double(double)>> funcs = {
            // Базовые тригонометрические функции
            {"sin", [](double x) { return std::sin(x); }},
            {"cos", [](double x) { return std::cos(x); }},
            {"tan", [](double x) { return std::tan(x); }},
            
            // Обратные тригонометрические функции
            {"asin", [](double x) { 
                if (x < -1.0 || x > 1.0) throw EvalError("asin: аргумент должен быть в диапазоне [-1, 1]");
                return std::asin(x); 
            }},
            {"acos", [](double x) { 
                if (x < -1.0 || x > 1.0) throw EvalError("acos: аргумент должен быть в диапазоне [-1, 1]");
                return std::acos(x); 
            }},
            {"atan", [](double x) { return std::atan(x); }},
            
            // Гиперболические функции
            {"sinh", [](double x) { return std::sinh(x); }},
            {"cosh", [](double x) { return std::cosh(x); }},
            {"tanh", [](double x) { return std::tanh(x); }},
            
            // Логарифмические функции
            {"log", [](double x) { 
                if (x <= 0.0) throw EvalError("log: аргумент должен быть положительным");
                return std::log(x); 
            }},
            {"ln", [](double x) { 
                if (x <= 0.0) throw EvalError("ln: аргумент должен быть положительным");
                return std::log(x); 
            }},
            {"log10", [](double x) { 
                if (x <= 0.0) throw EvalError("log10: аргумент должен быть положительным");
                return std::log10(x); 
            }},
            
            // Экспонента и корень
            {"exp", [](double x) { return std::exp(x); }},
            {"sqrt", [](double x) { 
                if (x < 0.0) throw EvalError("sqrt: аргумент должен быть неотрицательным");
                return std::sqrt(x); 
            }},
            
            // Дополнительные математические функции
            {"abs", [](double x) { return std::abs(x); }},
            {"ceil", [](double x) { return std::ceil(x); }},
            {"floor", [](double x) { return std::floor(x); }},
            {"round", [](double x) { return std::round(x); }},
            
            // Факториал (для целых чисел)
            {"factorial", [](double x) {
                if (x < 0.0) throw EvalError("factorial: аргумент должен быть неотрицательным");
                if (x != std::floor(x)) throw EvalError("factorial: аргумент должен быть целым числом");
                if (x > 170.0) throw EvalError("factorial: результат слишком велик");
                
                double result = 1.0;
                for (int i = 2; i <= static_cast<int>(x); ++i) {
                    result *= i;
                }
                return result;
            }}
        };
        
        auto it = funcs.find(name_);
        if (it == funcs.end()) {
            throw EvalError("Unknown function: " + name_);
        }
        
        return it->second(val);
    }
    
private:
    std::string name_;
    std::unique_ptr<Node> arg_;
};

} // namespace calc
