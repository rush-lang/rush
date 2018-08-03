#include "rush/lexer/lexer.hpp"
#include "rush/parser/parser.hpp"
#include <iostream>

char const* src = R"(
import std.io;

func main(args: string[]):
	println("hello world!");
	return 0;
)";

int main() {
	auto lxa = rush::lex(src);
	for (auto& tok : lxa) {
		std::cout << debug_string(tok) << std::endl;
	}
}