#pragma once

#ifndef RUSH_AST_STMTS_LOOP_HPP
#define RUSH_AST_STMTS_LOOP_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <memory>

namespace rush::ast {
   class for_statement;
   class while_statement;
} // rush::ast

namespace rush::ast::stmts {
   std::unique_ptr<ast::while_statement> while_(
      std::unique_ptr<ast::expression> cond,
      std::unique_ptr<ast::statement> body);
} // rush::ast::stmts

namespace rush::ast {
   class for_statement;
   class while_statement;

   class while_statement : public statement {
      struct factory_tag_t {};

      friend std::unique_ptr<ast::while_statement> stmts::while_(
         std::unique_ptr<ast::expression> cond,
         std::unique_ptr<ast::statement> body);

   public:
      while_statement(
         std::unique_ptr<ast::expression> cond,
         std::unique_ptr<ast::statement> body,
         factory_tag_t)
         : statement { ast::statement_kind::while_ }
         , _cond { std::move(cond) }
         , _body { std::move(body) } {}

      ast::expression const& condition() const noexcept {
         return*_cond;
      }

      ast::statement const& body() const noexcept {
         return *_body;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_while_stmt(*this);
      }

   private:
      std::unique_ptr<ast::expression> _cond;
      std::unique_ptr<ast::statement> _body;
   };
} // rush::ast


namespace rush::ast::stmts {

   inline std::unique_ptr<ast::while_statement> while_(
      std::unique_ptr<ast::expression> cond,
      std::unique_ptr<ast::statement> body) {
         return std::make_unique<ast::while_statement>(
            std::move(cond),
            std::move(body),
            ast::while_statement::factory_tag_t {});
      }
} // rush::ast::stmts

#endif // RUSH_AST_STMTS_LOOP_HPP
