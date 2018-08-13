#include "rush/ast/expr/unary.hpp"

namespace rush::ast {
	inline std::unique_ptr<unary_expression> make_unary_expression(
		std::unique_ptr<expression> operand,
		unary_operator opkind
	) {
		return std::make_unique<unary_expression>(
			std::move(operand),
			opkind, unary_expression::factory_tag_t{}
		);
	}
} // rush::ast

#define RUSH_UNARY_EXPRESSION_FACTORY_FUNC_DEFNS
#include "rush/ast/expr/_expressions.hpp"