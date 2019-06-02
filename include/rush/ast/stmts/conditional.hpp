#pragma once

#ifndef RUSH_AST_STMTS_CONDITIONAL_HPP
#define RUSH_AST_STMTS_CONDITIONAL_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/stmts/statement.hpp"

#include <memory>

namespace rush::ast {

   class conditional_statement;
   class alternating_statement;

   namespace stmts {
      std::unique_ptr<conditional_statement> if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then);

      std::unique_ptr<conditional_statement> if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then,
			std::unique_ptr<statement> else_);

      std::unique_ptr<conditional_statement> while_(
         std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then);
   }

   class conditional_statement : public statement {
      struct factory_tag_t {};

      friend std::unique_ptr<conditional_statement> stmts::if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then);

      friend std::unique_ptr<conditional_statement> stmts::while_(
         std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then);

   public:
      conditional_statement(
         ast::statement_kind kind,
         std::unique_ptr<expression> cond,
         std::unique_ptr<statement> body,
         factory_tag_t)
         : statement { kind }
         , _cond { std::move(cond) }
         , _body { std::move(body) } {}

      ast::expression const& condition() const noexcept {
         return *_cond;
      }

      ast::statement const& body() const noexcept {
         return *_body;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_conditional_stmt(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {
         _cond->attach(*this, context);
         _body->attach(*this, context);
      }

      virtual void detach(ast::node&, ast::context& context) override {
         _cond->detach(*this, context);
         _body->detach(*this, context);
      }

   private:
      std::unique_ptr<expression> _cond;
		std::unique_ptr<statement> _body;
   };


   class alternating_statement : public statement {
      struct factory_tag_t {};

      friend std::unique_ptr<conditional_statement> stmts::if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then,
			std::unique_ptr<statement> else_);

   public:
      alternating_statement(
         ast::statement_kind kind,
         std::unique_ptr<ast::statement> primary,
         std::unique_ptr<ast::statement> alternate,
         factory_tag_t)
         : statement { kind }
         , _primary { std::move(primary) }
         , _alternate { std::move(alternate) } {}

      ast::statement const& primary() const noexcept {
         return *_primary;
      }

      ast::statement const& alternate() const noexcept {
         return *_alternate;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_alternating_stmt(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {
         _primary->attach(*this, context);
         _alternate->attach(*this, context);
      }

      virtual void detach(ast::node&, ast::context& context) override {
         _primary->detach(*this, context);
         _alternate->detach(*this, context);
      }

   private:
      std::unique_ptr<ast::statement> _primary;
      std::unique_ptr<ast::statement> _alternate;
   };
}

namespace rush::ast::stmts {
   inline std::unique_ptr<conditional_statement> if_(
      std::unique_ptr<expression> cond,
      std::unique_ptr<statement> then) {
         return std::make_unique<conditional_statement>(
            statement_kind::if_,
            std::move(cond),
            std::move(then),
            conditional_statement::factory_tag_t {});
      }

   inline std::unique_ptr<conditional_statement> if_(
      std::unique_ptr<expression> cond,
      std::unique_ptr<statement> then,
      std::unique_ptr<statement> else_) {
         return if_(
            std::move(cond),
            std::make_unique<alternating_statement>(
               statement_kind::else_,
               std::move(then),
               std::move(else_),
               alternating_statement::factory_tag_t {}));
      }

   inline std::unique_ptr<conditional_statement> while_(
      std::unique_ptr<expression> cond,
		std::unique_ptr<statement> then) {
         return std::make_unique<conditional_statement>(
            statement_kind::while_,
            std::move(cond),
            std::move(then),
            conditional_statement::factory_tag_t {});
      }
}

#endif // RUSH_AST_STMTS_CONDITIONAL_HPP
