#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"
#include "rush/ast/ast.hpp"
#include <iostream>

char const* src1 = R"(let x = 2 + y * 3)";
char const* src2 = R"(var msg = "hello world")";

namespace ast = rush::ast;
namespace exprs = rush::ast::exprs;
namespace decls = rush::ast::decls;
namespace stmts = rush::ast::stmts;
namespace sema = rush::sema;

int main() {
	std::cout << src1 << std::endl;
	std::cout << "--------------------" << std::endl;
	rush::dump(src1);

	std::cout << std::endl << std::endl;
	std::cout << src2 << std::endl;
	std::cout << "--------------------" << std::endl;
	rush::dump(src2);

	std::cout << std::endl << std::endl;
	std::cout << "msg" << std::endl;
	std::cout << "--------------------" << std::endl;
	rush::dump("msg");
}
