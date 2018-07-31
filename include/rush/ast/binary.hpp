#pragma once

#ifndef RUSH_AST_BINARY_EXPRESSION
#define RUSH_AST_BINARY_EXPRESSION

#include "rush/ast/expression.hpp"

#include <memory>

namespace rush::ast {
	class binary_expression : public expression {
	public:
		binary_expression(std::unique_ptr<expression> left, std::unique_ptr<expression> right)
			: _left(this->allocate(std::move(left)))
			, _right(this->allocate(std::move(right))) {}

		binary_expression(binary_expression&& other)
			: _left(std::move(other._left))
			, _right(std::move(other._right)) {}

		binary_expression& operator = (binary_expression&& other) {
		}

		expression const& left() const noexcept { return *_left; }
		expression const& right() const noexcept { return *_right; }

		// virtual expression clone() const override {
		// 	return binary_expression { _left->clone(), _right->clone() };
		// }

		virtual void accept(ast::visitor&) const override {

		}
	private:
		expression* _left;
		expression* _right;
	};


	std::unique_ptr<binary_expression> addition_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
	std::unique_ptr<binary_expression> subtraction_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
	std::unique_ptr<binary_expression> multiplication_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
	std::unique_ptr<binary_expression> division_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
} // rush

#endif // RUSH_AST_BINARY_EXPRESSION
