#include <catch2/catch.hpp>
#include "rush/ast/context.hpp"
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/types/integral.hpp"
#include "rush/ast/decls/variable.hpp"
#include "rush/ast/decls/constant.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;
namespace decls = ast::decls;

auto const int32_type = ast::builtin_integral_type { ast::integral_kind::dword, true };

TEST_CASE( "rush::ast::constant_declaration", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto decl = decls::constant("abc", exprs::literal(1, ctx));

	REQUIRE( decl->name() == "abc" );
	REQUIRE( decl->type() == int32_type );
}

TEST_CASE( "rush::ast::variable_declaration", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto decl = decls::variable("def", exprs::literal(1, ctx));

	REQUIRE( decl->name() == "def" );
	REQUIRE( decl->type() == int32_type );
}
