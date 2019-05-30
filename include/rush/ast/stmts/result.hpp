#pragma once

#ifndef RUSH_AST_STMTS_RESULT_HPP
#define RUSH_AST_STMTS_RESULT_HPP

#include "rush/ast/stmts/simple.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/types/builtin.hpp"

#include <memory>
#include <cassert>

namespace rush::ast {
   class result_statement;

   namespace stmts {
      std::unique_ptr<ast::result_statement> throw_(std::unique_ptr<ast::expression>);
      std::unique_ptr<ast::result_statement> yield_(std::unique_ptr<ast::expression>);
      std::unique_ptr<ast::result_statement> return_(std::unique_ptr<ast::expression>);
   }

   class result_statement : public simple_statement {
      struct factory_tag_t {};
      friend std::unique_ptr<ast::result_statement> stmts::throw_(std::unique_ptr<ast::expression>);
      friend std::unique_ptr<ast::result_statement> stmts::yield_(std::unique_ptr<ast::expression>);
      friend std::unique_ptr<ast::result_statement> stmts::return_(std::unique_ptr<ast::expression>);
   public:
      result_statement(
         statement_kind kind,
         std::unique_ptr<ast::expression> expr,
         factory_tag_t)
         : simple_statement { kind, simple_statement::factory_tag_t {} }
         , _expr { std::move(expr) } {}


      ast::expression const& expression() const noexcept {
         return *_expr;
      }

      ast::type_ref result_type() const noexcept {
         return _expr->result_type();
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
         v.visit_result_stmt(*this);
		}

   private:
      std::unique_ptr<ast::expression> _expr;
   };
} // rush::ast

namespace rush::ast::stmts {
   inline std::unique_ptr<ast::result_statement> throw_(std::unique_ptr<ast::expression> expr) {
      assert(expr && "throw must have an expression.");
      return std::make_unique<ast::result_statement>(
         ast::statement_kind::return_, std::move(expr),
         ast::result_statement::factory_tag_t {});
   }

   inline std::unique_ptr<ast::result_statement> yield_(std::unique_ptr<ast::expression> expr) {
      assert(expr && "yield must have an expression.");
      return std::make_unique<ast::result_statement>(
         ast::statement_kind::return_, std::move(expr),
         ast::result_statement::factory_tag_t {});
   }

   inline std::unique_ptr<ast::result_statement> return_(std::unique_ptr<ast::expression> expr) {
      assert(expr && "return must have an expression.");
      return std::make_unique<ast::result_statement>(
         ast::statement_kind::return_, std::move(expr),
         ast::result_statement::factory_tag_t {});
   }

}

#endif // RUSH_AST_STMTS_RESULT_HPP
