#pragma once

#ifndef RUSH_AST_BINARY_HPP
#define RUSH_AST_BINARY_HPP

#include "rush/ast/expression.hpp"

#include <memory>

namespace rush::ast {
	enum class binary_operator {
		addition,
		subtraction,
		multiplication,
		division,
		modulo
	};

	class binary_expression : public expression {
		friend std::unique_ptr<binary_operator> make_binary_expr(
			std::unique_ptr<expression>,
			std::unique_ptr<expression>,
			binary_operator);

	public:
		binary_operator opkind() const noexcept { return _opkind; }
		expression const& left() const noexcept { return *_left; }
		expression const& right() const noexcept { return *_right; }

		virtual void accept(ast::visitor&) const override;

	private:
		std::unique_ptr<expression> _left;
		std::unique_ptr<expression> _right;
		binary_operator _opkind;

		binary_expression(std::unique_ptr<expression> left, std::unique_ptr<expression> right, binary_operator op)
			: _left(std::move(left))
			, _right(std::move(right))
			, _opkind(op) {}
	};

	std::unique_ptr<binary_expression> addition_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
	std::unique_ptr<binary_expression> subtraction_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
	std::unique_ptr<binary_expression> multiplication_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
	std::unique_ptr<binary_expression> division_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
	std::unique_ptr<binary_expression> modulo_expr(std::unique_ptr<expression>, std::unique_ptr<expression>);
} // rush

#endif // RUSH_AST_BINARY_HPP
