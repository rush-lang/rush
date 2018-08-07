#include <catch2/catch.hpp>
#include "rush/ast/expr/literal.hpp"

namespace ast = rush::ast;

TEST_CASE( "rush::literal_expression (boolean)", "[unit][ast]" ) {
	auto expr = ast::literal_expr(true);
}

TEST_CASE( "rush::literal_expression (string)", "[unit][ast]" ) {
	auto expr = ast::literal_expr("hello");
}

TEST_CASE( "rush::literal_expression (integral)", "[unit][ast]" ) {
	auto expr = ast::literal_expr(1, ast::int_type);
}

TEST_CASE( "rush::literal_expression (floating)", "[unit][ast]" ) {
	auto expr = ast::literal_expr(1.3, ast::float_type);
}
