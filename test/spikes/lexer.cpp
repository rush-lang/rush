#include "rush/lexer/lex.hpp"
#include "rush/lexer/dump.hpp"
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
	rush::dump(rush::lex(src));
}