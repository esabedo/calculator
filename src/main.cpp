#include <iostream>
#include <string>
#include <cstring>
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "error.hpp"

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] [expression]\n"
              << "Options:\n"
              << "  -h, --help    Show this help message\n"
              << "\n"
              << "If expression is provided, it will be evaluated.\n"
              << "Otherwise, a line is read from standard input.\n"
              << "\n"
              << "Examples:\n"
              << "  " << program_name << " \"2 + 3 * 4\"\n"
              << "  echo \"sin(pi/2)\" | " << program_name << "\n";
}

int main(int argc, char* argv[]) {
    std::string line;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        // If argument doesn't start with '-', treat it as expression
        if (argv[i][0] != '-') {
            line = argv[i];
            break;
        }
    }

    // If no expression provided, read from stdin
    if (line.empty()) {
        if (!std::getline(std::cin, line)) {
            return 0;
        }
    }

    try {
        calc::Lexer lexer(line);
        auto tokens = lexer.tokenize();

        calc::Parser parser(tokens);
        auto ast = parser.parse();

        calc::Evaluator evaluator;
        double result = evaluator.evaluate(ast);

        std::cout << result << std::endl;
        return 0;
    } catch (const calc::ParseError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const calc::EvalError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
