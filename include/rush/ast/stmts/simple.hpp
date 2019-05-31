#pragma once

#ifndef RUSH_AST_STMTS_SIMPLE_HPP
#define RUSH_AST_STMTS_SIMPLE_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/exprs/expression.hpp"

#include <memory>

namespace rush::ast {
   class simple_statement;
   class result_statement;

   namespace stmts {
      std::unique_ptr<ast::simple_statement> pass();
      std::unique_ptr<ast::simple_statement> throw_();
      std::unique_ptr<ast::simple_statement> break_();
      std::unique_ptr<ast::simple_statement> return_();
      std::unique_ptr<ast::simple_statement> continue_();
   }

   class simple_statement : public statement {
      struct factory_tag_t {};

      friend class result_statement;

      friend std::unique_ptr<simple_statement> stmts::pass();
      friend std::unique_ptr<ast::simple_statement> stmts::throw_();
      friend std::unique_ptr<ast::simple_statement> stmts::break_();
      friend std::unique_ptr<ast::simple_statement> stmts::return_();
      friend std::unique_ptr<ast::simple_statement> stmts::continue_();

   public:
      simple_statement(statement_kind kind, factory_tag_t)
         : statement { kind } {}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
         v.visit_simple_stmt(*this);
		}
   };
} // rush::ast

namespace rush::ast::stmts {
   inline std::unique_ptr<ast::simple_statement> pass() {
      return std::make_unique<ast::simple_statement>(
         ast::statement_kind::pass,
         ast::simple_statement::factory_tag_t {});
   }

   inline std::unique_ptr<ast::simple_statement> throw_() {
      return std::make_unique<ast::simple_statement>(
         ast::statement_kind::throw_,
         ast::simple_statement::factory_tag_t {});
   }

   inline std::unique_ptr<ast::simple_statement> break_() {
      return std::make_unique<ast::simple_statement>(
         ast::statement_kind::break_,
         ast::simple_statement::factory_tag_t {});
   }

   inline std::unique_ptr<ast::simple_statement> return_() {
      return std::make_unique<ast::simple_statement>(
         ast::statement_kind::return_,
         ast::simple_statement::factory_tag_t {});
   }

   inline std::unique_ptr<ast::simple_statement> continue_() {
      return std::make_unique<ast::simple_statement>(
         ast::statement_kind::continue_,
         ast::simple_statement::factory_tag_t {});
   }
}

#endif // RUSH_AST_STMTS_SIMPLE_HPP
