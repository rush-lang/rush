#pragma once

#ifndef RUSH_AST_BINARY_HPP
#define RUSH_AST_BINARY_HPP

#include "rush/ast/expression.hpp"
#include "rush/sema/types.hpp"

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

		virtual ast::type result_type() const override {
			switch (opkind()) {
			case binary_operator::logical_or:
			case binary_operator::logical_and:
			case binary_operator::less_than:
			case binary_operator::less_equals:
			case binary_operator::greater_than:
			case binary_operator::greater_equals:
				return ast::bool_type;

			default:
				auto lhs = left_operand().result_type().symbol();
				auto rhs = right_operand().result_type().symbol();

				if (lhs.is_type() && rhs.is_type()) {
					auto tred = sema::reduce_type_symbols(lhs, rhs);
					return { std::get<sema::symbol>(tred) };
				}
			}
			return ast::error_type;
		}

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

#endif // RUSH_AST_BINARY_HPP
