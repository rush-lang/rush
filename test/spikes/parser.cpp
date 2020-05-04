#include "rush/lexer/lex.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"
#include "rush/ast/ast.hpp"
#include <iostream>

char const* src = R"(
import std.io;

func main(args: string[]):
   println("hello world!");
   return 0;
)";

namespace ast = rush::ast;
namespace exprs = rush::ast::exprs;
namespace decls = rush::ast::decls;
namespace stmts = rush::ast::stmts;
namespace types = rush::ast::types;

int main() {
	std::cout << src << std::endl;
	std::cout << "--------------------" << std::endl;
	rush::dump(src);
}
