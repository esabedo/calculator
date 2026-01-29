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
            {"sin", [](double x) { return std::sin(x); }},
            {"cos", [](double x) { return std::cos(x); }},
            {"tan", [](double x) { return std::tan(x); }},
            {"log", [](double x) { 
                if (x <= 0.0) throw EvalError("log: argument must be positive");
                return std::log(x); 
            }},
            {"exp", [](double x) { return std::exp(x); }},
            {"sqrt", [](double x) { 
                if (x < 0.0) throw EvalError("sqrt: argument must be non-negative");
                return std::sqrt(x); 
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
