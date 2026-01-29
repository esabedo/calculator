#include <gtest/gtest.h>
#include <cmath>
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "error.hpp"

using namespace calc;

// Constants for testing
const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;

// Helper function to evaluate an expression
double evaluate_expression(const std::string& expr) {
    Lexer lexer(expr);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    Evaluator evaluator;
    return evaluator.evaluate(ast);
}

// Basic arithmetic tests
TEST(CalculatorTest, Addition) {
    EXPECT_DOUBLE_EQ(evaluate_expression("2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("0 + 0"), 0.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("-5 + 10"), 5.0);
}

TEST(CalculatorTest, Subtraction) {
    EXPECT_DOUBLE_EQ(evaluate_expression("10 - 3"), 7.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("5 - 10"), -5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("0 - 0"), 0.0);
}

TEST(CalculatorTest, Multiplication) {
    EXPECT_DOUBLE_EQ(evaluate_expression("4 * 5"), 20.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("0 * 100"), 0.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("-3 * 4"), -12.0);
}

TEST(CalculatorTest, Division) {
    EXPECT_DOUBLE_EQ(evaluate_expression("20 / 4"), 5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("10 / 3"), 10.0 / 3.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("-15 / 3"), -5.0);
}

TEST(CalculatorTest, Power) {
    EXPECT_DOUBLE_EQ(evaluate_expression("2 ^ 3"), 8.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("3 ^ 2"), 9.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("2 ^ 0"), 1.0);
}

// Complex expressions with parentheses
TEST(CalculatorTest, Parentheses) {
    EXPECT_DOUBLE_EQ(evaluate_expression("(2 + 3) * 4"), 20.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("2 * (3 + 4)"), 14.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("(10 - 5) / (2 + 3)"), 1.0);
}

TEST(CalculatorTest, ComplexExpression) {
    EXPECT_DOUBLE_EQ(evaluate_expression("3 + 4 * 2 / (1 - 5) ^ 2 ^ 3"), 3.0001220703125);
}

// Mathematical functions
TEST(CalculatorTest, SinFunction) {
    EXPECT_NEAR(evaluate_expression("sin(0)"), 0.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("sin(1.57079632679)"), 1.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("sin(3.14159265359)"), 0.0, 1e-10);
}

TEST(CalculatorTest, CosFunction) {
    EXPECT_NEAR(evaluate_expression("cos(0)"), 1.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("cos(1.57079632679)"), 0.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("cos(3.14159265359)"), -1.0, 1e-10);
}

TEST(CalculatorTest, TanFunction) {
    EXPECT_NEAR(evaluate_expression("tan(0)"), 0.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("tan(0.78539816339)"), 1.0, 1e-10);
}

TEST(CalculatorTest, LogFunction) {
    EXPECT_NEAR(evaluate_expression("log(1)"), 0.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("log(2.71828182846)"), 1.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("log(10)"), 2.302585092994046, 1e-10);
}

TEST(CalculatorTest, ExpFunction) {
    EXPECT_NEAR(evaluate_expression("exp(0)"), 1.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("exp(1)"), 2.718281828459045, 1e-10);
}

TEST(CalculatorTest, SqrtFunction) {
    EXPECT_DOUBLE_EQ(evaluate_expression("sqrt(4)"), 2.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("sqrt(9)"), 3.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("sqrt(0)"), 0.0);
}

// Nested functions
TEST(CalculatorTest, NestedFunctions) {
    EXPECT_NEAR(evaluate_expression("sin(cos(0))"), std::sin(1.0), 1e-10);
    EXPECT_NEAR(evaluate_expression("sqrt(sqrt(16))"), 2.0, 1e-10);
    EXPECT_NEAR(evaluate_expression("log(exp(5))"), 5.0, 1e-10);
}

// Unary operators
TEST(CalculatorTest, UnaryMinus) {
    EXPECT_DOUBLE_EQ(evaluate_expression("-5"), -5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("-(-5)"), 5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("3 + -2"), 1.0);
}

TEST(CalculatorTest, UnaryPlus) {
    EXPECT_DOUBLE_EQ(evaluate_expression("+5"), 5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("3 + +2"), 5.0);
}

// Error handling tests
TEST(CalculatorTest, DivisionByZero) {
    EXPECT_THROW(evaluate_expression("1 / 0"), EvalError);
}

TEST(CalculatorTest, InvalidExpression) {
    EXPECT_THROW(evaluate_expression("2 + * 3"), ParseError);
}

TEST(CalculatorTest, MismatchedParentheses) {
    EXPECT_THROW(evaluate_expression("(2 + 3"), ParseError);
    EXPECT_THROW(evaluate_expression("2 + 3)"), ParseError);
}

TEST(CalculatorTest, UnknownFunction) {
    EXPECT_THROW(evaluate_expression("unknown(5)"), EvalError);
}

// Whitespace handling
TEST(CalculatorTest, WhitespaceHandling) {
    EXPECT_DOUBLE_EQ(evaluate_expression("  2  +  3  "), 5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("2+3"), 5.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("  sin(  1.57079632679  )  "), 1.0);
}

// Decimal numbers
TEST(CalculatorTest, DecimalNumbers) {
    EXPECT_DOUBLE_EQ(evaluate_expression("3.14 + 2.86"), 6.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("0.5 * 4"), 2.0);
    EXPECT_NEAR(evaluate_expression("3.14159"), 3.14159, 1e-5);
}

// Operator precedence
TEST(CalculatorTest, OperatorPrecedence) {
    EXPECT_DOUBLE_EQ(evaluate_expression("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("2 * 3 + 4"), 10.0);
    EXPECT_DOUBLE_EQ(evaluate_expression("2 ^ 3 ^ 2"), 512.0);  // Right associative
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
