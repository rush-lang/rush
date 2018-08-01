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


	std::unique_ptr<binary_expression> addition_expr(std::unique_ptr<expression> lhs, std::unique_ptr<expression> rhs) {
		return make_binary_expr(std::move(lhs), std::move(rhs), binary_operator::addition);
	}

	std::unique_ptr<binary_expression> subtraction_expr(std::unique_ptr<expression> lhs, std::unique_ptr<expression> rhs) {
		return make_binary_expr(std::move(lhs), std::move(rhs), binary_operator::subtraction);
	}

	std::unique_ptr<binary_expression> multiplication_expr(std::unique_ptr<expression> lhs, std::unique_ptr<expression> rhs) {
		return make_binary_expr(std::move(lhs), std::move(rhs), binary_operator::multiplication);
	}

	std::unique_ptr<binary_expression> division_expr(std::unique_ptr<expression> lhs, std::unique_ptr<expression> rhs) {
		return make_binary_expr(std::move(lhs), std::move(rhs), binary_operator::division);
	}

	std::unique_ptr<binary_expression> modulo_expr(std::unique_ptr<expression> lhs, std::unique_ptr<expression> rhs) {
		return make_binary_expr(std::move(lhs), std::move(rhs), binary_operator::modulo);
	}
} // rush