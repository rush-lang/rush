#include "catch2/catch.hpp"
#include "rush/parser/parse.hpp"
#include "rush/ast/expr/literal.hpp"

#include <string>

namespace ast = rush::ast;

bool valid_parse(std::string input, std::unique_ptr<ast::node> root) {
	auto ast = rush::parse(input);
	return true;
}

TEST_CASE("rush::parse", "[unit][parser]") {
	// CHECK( valid_parse("let i = 0", ast::constant_decl("i", ast::int_type, ast::literal_expr(0))));
	// CHECK( valid_parse("let d = 0.1", ast::constant_decl("d", ast::double_type, ast::literal_expr(0))));
	// CHECK( valid_parse("let s = \"abc\"", ast::constant_decl("s", ast::string_type, ast::literal_expr(0))));
}

TEST_CASE( "rush::parse (expressions)", "[unit][parser]" ) {
	// auto ast = rush::parse("123");
	SECTION( "integer literals" ) {
		CHECK( valid_parse("0", ast::literal_expr(0, ast::int_type)) );
		CHECK( valid_parse("1", ast::literal_expr(1, ast::int_type)) );
		CHECK( valid_parse("9", ast::literal_expr(9, ast::int_type)) );
		CHECK( valid_parse("123", ast::literal_expr(9, ast::int_type)) );

		CHECK( valid_parse("345u", ast::literal_expr(9, ast::uint_type)) );
		CHECK( valid_parse("645l", ast::literal_expr(9, ast::long_type)) );
		CHECK( valid_parse("645ul", ast::literal_expr(9, ast::ulong_type)) );
	}
}
