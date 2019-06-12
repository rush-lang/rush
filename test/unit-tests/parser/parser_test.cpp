#include "catch2/catch.hpp"
#include "rush/parser/parse.hpp"
#include "rush/ast/exprs/binary.hpp"
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/types/integral.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace ast = rush::ast;
namespace exprs = ast::exprs;

bool valid_parse(std::string input, std::unique_ptr<ast::node> root) {
	auto ast = rush::parse(input);
	return true;
}

TEST_CASE("rush::parse", "[unit][parser]") {
	// CHECK( valid_parse("let i = 0", decls::constant("i", ctx, exprs::literal(0))));
	// CHECK( valid_parse("let d = 0.1", decls::constant("d", ieee64_type, exprs::literal(0))));
	// CHECK( valid_parse("let s = \"abc\"", decls::constant("s", string_type, exprs::literal(0))));
}

TEST_CASE( "rush::parse (expressions)", "[unit][parser]" ) {
   auto ctx = ast::context {};

	// auto ast = rush::parse("123");
	SECTION( "integer literals" ) {
		CHECK( valid_parse("0", exprs::literal(0, ctx)) );
		CHECK( valid_parse("1", exprs::literal(1, ctx)) );
		CHECK( valid_parse("9", exprs::literal(9, ctx)) );
		CHECK( valid_parse("123", exprs::literal(9, ctx)) );

		CHECK( valid_parse("345u", exprs::literal(9, ctx)) );
		CHECK( valid_parse("645l", exprs::literal(9, ctx)) );
		CHECK( valid_parse("645ul", exprs::literal(9, ctx)) );
	}

	SECTION( "binary expressions" ) {
		CHECK( valid_parse("1 + 1",
			exprs::addition(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));

		CHECK( valid_parse("2 - 1",
			exprs::subtraction(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));

		CHECK( valid_parse("x * 2",
			exprs::multiplication(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));

		CHECK( valid_parse("x / y",
			exprs::division(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));
	}
}
