#include "catch2/catch.hpp"
#include "rush/parser/parser.hpp"

#include <string>

namespace ast = rush::ast;

bool valid_parse(std::string input, std::unique_ptr<ast::node> root) {
	return false;
}

TEST_CASE("rush::parse", "[unit][parser]") {
	// CHECK( valid_parse("let i = 0", ast::constant_decl("i", ast::int_type, ast::literal_expr(0))));
	// CHECK( valid_parse("let d = 0.1", ast::constant_decl("d", ast::double_type, ast::literal_expr(0))));
	// CHECK( valid_parse("let s = \"abc\"", ast::constant_decl("s", ast::string_type, ast::literal_expr(0))));
}
