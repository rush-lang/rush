/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#pragma once

#ifndef RUSH_AST_STMTS_FORWARDING_HPP
#define RUSH_AST_STMTS_FORWARDING_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/stmts/expression.hpp"
#include "rush/ast/stmts/declaration.hpp"

#include <variant>
#include <memory>

namespace rush::ast::detail {
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
		}

	private:
		std::unique_ptr<statement> _stmt;
	};
}

#endif // RUSH_AST_STMTS_FORWARDING_HPP
