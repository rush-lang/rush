#pragma once

#ifndef RUSH_AST_STMTS_BRANCH_HPP
#define RUSH_AST_STMTS_BRANCH_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/stmts/statement.hpp"

namespace rush::ast {
	class if_statement : public statement {
   public:
      if_statement(
         std::unique_ptr<expression> cond,
         std::unique_ptr<statement> then,
         std::unique_ptr<statement> else_)
         : _cond { std::move(cond) }
         , _then { std::move(then) }
         , _else { std::move(else_) } {}

		virtual ast::statement_kind kind() const noexcept override {
         return ast::statement_kind::branch;
      }

      ast::expression const& condition() const noexcept {
         return *_cond;
      }

      ast::statement const& then() const noexcept {
         return *_then;
      }

      ast::statement const* else_() const noexcept {
         return _else.get();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_if_stmt(*this);
      }

	private:
		std::unique_ptr<expression> _cond;
		std::unique_ptr<statement> _then;
		std::unique_ptr<statement> _else;
	};

	class case_statement {
	private:

	};

	class switch_statement {
	private:
		std::unique_ptr<expression> _expr;
		std::vector<std::vector<case_statement>> _cases;
	};

	namespace stmts {
		inline std::unique_ptr<if_statement> if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then,
			std::unique_ptr<statement> else_) {
            return std::make_unique<if_statement>(
               std::move(cond),
               std::move(then),
               std::move(else_));
         }

      inline std::unique_ptr<if_statement> if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then) {
            return if_(
               std::move(cond),
               std::move(then),
               nullptr);
         }


		std::unique_ptr<case_statement> case_(
			// some pattern type,
			std::unique_ptr<statement> body);

		std::unique_ptr<switch_statement> switch_(
			std::unique_ptr<expression> expr,
			std::initializer_list<std::unique_ptr<case_statement>> cases);
	}
}

#endif // RUSH_AST_STMTSS_BRANCH_HPP
