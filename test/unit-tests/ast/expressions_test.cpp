#include <catch2/catch.hpp>
#include "rush/ast/exprs/literal.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;

TEST_CASE( "rush::literal_expression (boolean)", "[unit][ast]" ) {
	auto expr = exprs::literal(true);
}

TEST_CASE( "rush::literal_expression (string)", "[unit][ast]" ) {
	auto expr = exprs::literal("hello");
}

TEST_CASE( "rush::literal_expression (integral)", "[unit][ast]" ) {
	auto expr = exprs::literal(1, ast::int_type);
}

TEST_CASE( "rush::literal_expression (floating)", "[unit][ast]" ) {
	auto expr = exprs::literal(1.3, ast::float_type);
}
