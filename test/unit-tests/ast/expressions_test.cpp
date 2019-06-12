#include <catch2/catch.hpp>
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/integral.hpp"
#include "rush/ast/types/floating.hpp"
#include "rush/ast/exprs/literal.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;

auto const bool_type = ast::builtin_bool_type {};
auto const int32_type = ast::builtin_integral_type { ast::integral_kind::dword, true };
auto const ieee32_type = ast::builtin_floating_point_type { ast::floating_point_kind::ieee32 };

TEST_CASE( "rush::literal_expression (boolean)", "[unit][ast]" ) {
	auto expr = exprs::literal(true, bool_type);
}

// TEST_CASE( "rush::literal_expression (string)", "[unit][ast]" ) {
// 	auto expr = exprs::literal("hello", string_type);
// }

TEST_CASE( "rush::literal_expression (integral)", "[unit][ast]" ) {
	auto expr = exprs::literal(1, int32_type);
}

TEST_CASE( "rush::literal_expression (floating)", "[unit][ast]" ) {
	auto expr = exprs::literal(1.3, ieee32_type);
}
