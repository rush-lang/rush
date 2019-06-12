#include <catch2/catch.hpp>
#include "rush/ast/context.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/integral.hpp"
#include "rush/ast/types/floating.hpp"
#include "rush/ast/exprs/literal.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;

TEST_CASE( "rush::literal_expression (boolean)", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto expr = exprs::literal(true, ctx);
}

// TEST_CASE( "rush::literal_expression (string)", "[unit][ast]" ) {
// 	auto expr = exprs::literal("hello", string_type);
// }

TEST_CASE( "rush::literal_expression (integral)", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto expr = exprs::literal(1, ctx);
}

TEST_CASE( "rush::literal_expression (floating)", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto expr = exprs::literal(1.3, ctx);
}
