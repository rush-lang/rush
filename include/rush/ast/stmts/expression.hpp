#pragma once

#ifndef RUSH_AST_STMTS_EXPRESSION_HPP
#define RUSH_AST_STMTS_EXPRESSION_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/exprs/expression.hpp"

#include "rush/ast/visitor.hpp"

namespace rush::ast::details {

	// simple wrapper around expressions so that some
	// expressions may be treated as statements
	class expression_statement : public statement {
	public:
		expression_statement(std::unique_ptr<expression> expr)
			: _expr(std::move(expr)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::expression;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			_expr->accept(v);
		}

	private:
		std::unique_ptr<expression> _expr;
	};

	inline std::unique_ptr<expression_statement> expr_stmt(std::unique_ptr<expression> expr) {
		if (expr == nullptr) throw new std::invalid_argument("expr == nullptr");
		return std::make_unique<expression_statement>(std::move(expr));
	}
} // rush::ast::details

#endif // RUSH_AST_STMTS_EXPRESSION_HPP