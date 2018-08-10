#include <catch2/catch.hpp>
#include "rush/ast/expr/literal.hpp"
#include "rush/ast/decl/variable.hpp"
#include "rush/ast/decl/constant.hpp"

namespace ast = rush::ast;

TEST_CASE( "rush::ast::constant_declaration", "[unit][ast]" ) {
	auto decl = ast::constant_decl("abc", ast::literal_expr(1, ast::int_type));

	REQUIRE( decl->name() == "abc" );
	REQUIRE( decl->type() == ast::int_type );
}

TEST_CASE( "rush::ast::variable_declaration", "[unit][ast]" ) {
	auto decl = ast::variable_decl("def", ast::literal_expr(1, ast::int_type));

	REQUIRE( decl->name() == "def" );
	REQUIRE( decl->type() == ast::int_type );
}