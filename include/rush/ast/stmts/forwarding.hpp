#pragma once

#ifndef RUSH_AST_STMTS_FORWARDING_HPP
#define RUSH_AST_STMTS_FORWARDING_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/stmts/expression.hpp"
#include "rush/ast/stmts/declaration.hpp"

#include <variant>
#include <memory>

namespace rush::ast::details {
	// wraps declarations and expressions in a statement,
	// enable the former to be implicitly converted statement
	// and in the case of a genuine statement, just move.
	class statement_fwdr {
	public:
		template <typename Stmt>
		statement_fwdr(std::unique_ptr<Stmt> stmt,
			std::enable_if_t<std::is_base_of_v<statement, Stmt>>* = nullptr)
			: _stmt(std::move(stmt)) {}

		template <typename Expr>
		statement_fwdr(std::unique_ptr<Expr> expr,
			std::enable_if_t<std::is_base_of_v<expression, Expr>>* = nullptr)
			: _stmt(ast::stmts::expr_stmt(std::move(expr))) {}

		template <typename Decl>
		statement_fwdr(std::unique_ptr<Decl> decl,
			std::enable_if_t<std::is_base_of_v<declaration, Decl>>* = nullptr)
			: _stmt(ast::stmts::decl_stmt(std::move(decl))) {}

		operator std::unique_ptr<statement>() && {
			return std::move(_stmt);
			// return std::visit(wrap_visitor {}, _var);
		}

	private:
		std::unique_ptr<statement> _stmt;
	};
}

#endif // RUSH_AST_STMTS_FORWARDING_HPP
