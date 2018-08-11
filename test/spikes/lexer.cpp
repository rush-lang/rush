#include "rush/lexer/lex.hpp"
#include <iostream>

char const* src = R"(
import std.io;

func main(args: string[]):
   println("hello world!");
   return 0;
)";

int main() {
	std::cout << src << std::endl;
	std::cout << "output: --------------------" << std::endl;
	auto lxa = rush::lex(src);
	for (auto& tok : lxa) {
		std::cout << debug_string(tok) << std::endl;
	}
}