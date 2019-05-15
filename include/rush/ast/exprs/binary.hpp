#pragma once

#ifndef RUSH_AST_EXPRS_BINARY_HPP
#define RUSH_AST_EXPRS_BINARY_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>

#define RUSH_BINARY_OPERATORS_ENUM
#include "rush/ast/_operators.hpp"

namespace rush::ast {
	class binary_expression : public expression {
		struct factory_tag_t {};

	public:
		binary_expression(
			std::unique_ptr<expression> left,
			std::unique_ptr<expression> right,
			binary_operator op, factory_tag_t)
			: _left(std::move(left))
			, _right(std::move(right))
			, _opkind(op) {}

		binary_operator opkind() const noexcept { return _opkind; }
		expression const& left_operand() const noexcept { return *_left; }
		expression const& right_operand() const noexcept { return *_right; }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_binary_expr(*this);
#			define RUSH_BINARY_EXPRESSION_VISIT_SWITCH
#			include "rush/ast/_operators.hpp"
		}

	private:
		std::unique_ptr<expression> _left;
		std::unique_ptr<expression> _right;
		binary_operator _opkind;

		friend std::unique_ptr<binary_expression> make_binary_expr(
			std::unique_ptr<expression> left,
			std::unique_ptr<expression> right,
			binary_operator op
		);
	};
} // rush

#define RUSH_BINARY_EXPRESSION_FACTORY_FUNC_DECLS
#include "rush/ast/exprs/_expressions.hpp"

#endif // RUSH_AST_EXPRS_BINARY_HPP
