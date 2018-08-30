#pragma once

#ifndef RUSH_AST_STMT_RETURN_HPP
#define RUSH_AST_STMT_RETURN_HPP

#include "rush/ast/statement.hpp"
#include "rush/ast/expression.hpp"
#include "rush/ast/type.hpp"

namespace rush::ast {

	class return_statement;
	namespace stmts {
		std::unique_ptr<return_statement> return_(
			std::unique_ptr<expression> expr);
	}

	class return_statement : public statement {
		struct factory_tag_t {};

		friend std::unique_ptr<return_statement> stmts::return_(
			std::unique_ptr<expression> expr);

	public:
		return_statement(std::unique_ptr<expression> expr, factory_tag_t)
			: _expr(std::move(expr)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::control;
		}

		ast::type type() const noexcept {
			return _expr != nullptr
				? _expr->result_type()
				: ast::void_type;
		}

		expression const* const expr() const noexcept {
			return _expr.get();
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {

		}

	private:
		std::unique_ptr<expression> _expr;
	};

	namespace stmts {
		inline std::unique_ptr<return_statement> return_(
			std::unique_ptr<expression> expr) {
				return std::make_unique<return_statement>(
					std::move(expr), return_statement::factory_tag_t {});
			}

		inline std::unique_ptr<return_statement> return_() {
			return stmts::return_(nullptr);
		}
	}
} // rush::ast

#endif // RUSH_AST_STMT_RETURN_HPP
