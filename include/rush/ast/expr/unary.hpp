#pragma once

#ifndef RUSH_AST_UNARY_HPP
#define RUSH_AST_UNARY_HPP

#include "rush/ast/expression.hpp"

#include <memory>

#define RUSH_UNARY_OPERATOR_ENUM
#include "rush/ast/_operators.hpp"

namespace rush::ast {
	class unary_expression : public expression {
		struct factory_tag_t {};

	public:
		unary_expression(
			std::unique_ptr<expression> operand,
			unary_operator opkind, factory_tag_t)
			: _operand(std::move(operand))
			, _opkind(opkind) {}

		unary_operator opkind() const noexcept { return _opkind; }
		expression const& operand() const noexcept { return *_operand; }

		virtual ast::type result_type() const override {
			return _operand->result_type();
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_unary_expr(*this);
		}

	private:
		std::unique_ptr<expression> _operand;
		unary_operator _opkind;

		friend std::unique_ptr<unary_expression> make_unary_expression(
			std::unique_ptr<expression>,
			unary_operator);
	};

	std::unique_ptr<unary_expression> negate_expr(std::unique_ptr<expression>);
	std::unique_ptr<unary_expression> increment_expr(std::unique_ptr<identifier_expression>);
	std::unique_ptr<unary_expression> decrement_expr(std::unique_ptr<identifier_expression>);
} // rush

#endif // RUSH_AST_UNARY_HPP
