#include "rush/lexer/lex.hpp"
#include "rush/parser/parse.hpp"
#include "rush/ast/printer.hpp"
#include "rush/ast/expression.hpp"
#include <iostream>

char const* src = R"(
import std.io;

func main(args: string[]):
	println("hello world!");
	return 0;
)";

namespace ast = rush::ast;
int main() {
	auto pr = rush::parse("10 + 2 * 3 + 4");
	pr.ast()->accept(ast::printer { std::cout });

	auto lxa = rush::lex(src);
	for (auto& tok : lxa) {
		std::cout << debug_string(tok) << std::endl;
	}
}