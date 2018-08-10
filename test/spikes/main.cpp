#include "rush/lexer/lex.hpp"
#include "rush/parser/parse.hpp"
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
	rush::dump("10 + 2 * 3 + 4");
	auto lxa = rush::lex(src);
	for (auto& tok : lxa) {
		std::cout << debug_string(tok) << std::endl;
	}
}