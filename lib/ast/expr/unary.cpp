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

	std::unique_ptr<unary_expression> negate_expr(std::unique_ptr<expression> operand) {
		return make_unary_expression(std::move(operand), unary_operator::negate);
	}

	std::unique_ptr<unary_expression> increment_expr(std::unique_ptr<identifier_expression> operand) {
		return make_unary_expression(std::move(operand), unary_operator::increment);
	}

	std::unique_ptr<unary_expression> decrement_expr(std::unique_ptr<identifier_expression> operand) {
		return make_unary_expression(std::move(operand), unary_operator::decrement);
	}
} // rush::ast