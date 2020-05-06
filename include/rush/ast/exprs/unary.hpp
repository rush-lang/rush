#pragma once

#ifndef RUSH_AST_EXPRS_UNARY_HPP
#define RUSH_AST_EXPRS_UNARY_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>

#define RUSH_UNARY_OPERATOR_ENUM
#include "rush/ast/exprs/_operators.hpp"

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

      virtual ast::type_ref result_type() const noexcept override {
         return _operand->result_type();
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_unary_expr(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {
         _operand->attach(*this, context);
      }

      virtual void detach(ast::node&, ast::context& context) override {
         _operand->detach(*this, context);
      }

	private:
		std::unique_ptr<expression> _operand;
		unary_operator _opkind;

		friend std::unique_ptr<unary_expression> make_unary_expression(
			std::unique_ptr<expression>,
			unary_operator);
	};
} // rush

#define RUSH_UNARY_EXPRESSION_FACTORY_FUNC_DECLS
#include "rush/ast/exprs/_expressions.hpp"

#endif // RUSH_AST_EXPRS_UNARY_HPP
