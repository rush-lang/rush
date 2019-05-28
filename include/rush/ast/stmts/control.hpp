#pragma once

#ifndef RUSH_AST_STMTS_RETURN_HPP
#define RUSH_AST_STMTS_RETURN_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/exprs/expression.hpp"

namespace rush::ast {

	class return_statement;
	namespace stmts {
      std::unique_ptr<break_statement> break_();
      std::unique_ptr<continue_statement> continue_();

      std::unique_ptr<yield_statement> yield_(std::unique_ptr<ast::expression>);
		std::unique_ptr<return_statement> return_(std::unique_ptr<ast::expression>);
	}

   class break_statement : public statement {
      struct factory_tag_t {};
      friend std::unique_ptr<break_statement> stmts::break_();
   public:
      break_statement(factory_tag_t) {}

      virtual statement_kind kind() const noexcept override {
			return statement_kind::control;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
         v.visit_break_stmt(*this);
		}
   };

   class continue_statement : public statement {
      struct factory_tag_t {};
      friend std::unique_ptr<continue_statement> stmts::continue_();
   public:
      continue_statement(factory_tag_t) {}

      virtual statement_kind kind() const noexcept override {
			return statement_kind::control;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
         v.visit_continue_stmt(*this);
		}
   };

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

		expression const* const expr() const noexcept {
			return _expr.get();
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
         v.visit_return_stmt(*this);
		}

	private:
		std::unique_ptr<expression> _expr;
	};


   class yield_statement : public statement {
      struct factory_tag_t {};

      friend std::unique_ptr<yield_statement> stmts::yield_(
         std::unique_ptr<ast::expression>);
   public:
      yield_statement(std::unique_ptr<ast::expression> expr, factory_tag_t)
			: _expr(std::move(expr)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::control;
		}

		expression const& expr() const noexcept {
			return *_expr;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
         v.visit_yield_stmt(*this);
		}

	private:
		std::unique_ptr<expression> _expr;
   };


	namespace stmts {

      inline std::unique_ptr<break_statement> break_() {
         return std::make_unique<break_statement>(break_statement::factory_tag_t {});
      }

      inline std::unique_ptr<continue_statement> continue_() {
         return std::make_unique<continue_statement>(continue_statement::factory_tag_t {});
      }

		inline std::unique_ptr<return_statement> return_(
			std::unique_ptr<expression> expr) {
				return std::make_unique<return_statement>(
					std::move(expr), return_statement::factory_tag_t {});
			}

		inline std::unique_ptr<return_statement> return_() {
			return stmts::return_(nullptr);
		}

      inline std::unique_ptr<yield_statement> yield_(
         std::unique_ptr<ast::expression> expr) {
            return std::make_unique<yield_statement>(
               std::move(expr), yield_statement::factory_tag_t {});
         }
	}
} // rush::ast

#endif // RUSH_AST_STMTS_RETURN_HPP
