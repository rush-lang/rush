#include "rush/ast/expr/binary.hpp"

namespace rush::ast {
	inline std::unique_ptr<binary_expression> make_binary_expr(
		std::unique_ptr<expression> left,
		std::unique_ptr<expression> right,
		binary_operator op
	) {
		return std::make_unique<binary_expression>(
			std::move(left),
			std::move(right),
			op, binary_expression::factory_tag_t{}
		);
	}
} // rush::ast

#define RUSH_BINARY_EXPRESSION_FACTORY_FUNC_DEFNS
#include "rush/ast/expr/_expressions.hpp"