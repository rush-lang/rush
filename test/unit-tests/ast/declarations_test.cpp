#include <catch2/catch.hpp>
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/decls/variable.hpp"
#include "rush/ast/decls/constant.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;
namespace decls = ast::decls;

TEST_CASE( "rush::ast::constant_declaration", "[unit][ast]" ) {
	auto decl = decls::constant("abc", exprs::literal(1, ast::int_type));

	REQUIRE( decl->name() == "abc" );
	REQUIRE( decl->type() == ast::int_type );
}

TEST_CASE( "rush::ast::variable_declaration", "[unit][ast]" ) {
	auto decl = decls::variable("def", exprs::literal(1, ast::int_type));

	REQUIRE( decl->name() == "def" );
	REQUIRE( decl->type() == ast::int_type );
}