#include "rush/parser/parse.hpp"
#include <iostream>

char const* src1 = R"(let x = 2 + y * 3)";
char const* src2 = R"(let msg = "hello world")";

namespace ast = rush::ast;
int main() {
	std::cout << src1 << std::endl;
	std::cout << "--------------------" << std::endl;
	rush::dump(src1);

	std::cout << std::endl << std::endl;
	std::cout << src2 << std::endl;
	std::cout << "--------------------" << std::endl;
	rush::dump(src2);
}