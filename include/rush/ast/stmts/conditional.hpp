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

#ifndef RUSH_AST_STMTS_CONDITIONAL_HPP
#define RUSH_AST_STMTS_CONDITIONAL_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/scope.hpp"

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
         : _cond { std::move(cond) }
         , _body { std::move(body) }
         , _kind { kind } {
            adopt(*_cond);
            adopt(*_body);
         }

      virtual ast::statement_kind kind() const noexcept override {
         return _kind;
      }

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

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         scope.push(ast::scope_kind::block);
         attach(scope, *_cond);
         attach(scope, *_body);
         scope.pop();
      }

      virtual void detached(ast::context&) override {
         detach(*_cond);
         detach(*_body);
      }

   private:
      std::unique_ptr<expression> _cond;
		std::unique_ptr<statement> _body;
      ast::statement_kind _kind;
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
         : _primary { std::move(primary) }
         , _alternate { std::move(alternate) }
         , _kind { kind } {
            adopt(*_primary);
            adopt(*_alternate);
         }

      virtual ast::statement_kind kind() const noexcept override {
         return _kind;
      }

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

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         scope.push(ast::scope_kind::block);
         attach(scope, *_primary);
         scope.pop();

         scope.push(ast::scope_kind::block);
         attach(scope, *_alternate);
         scope.pop();
      }

      virtual void detached(ast::context&) override {
         detach(*_primary);
         detach(*_alternate);
      }

   private:
      std::unique_ptr<ast::statement> _primary;
      std::unique_ptr<ast::statement> _alternate;
      ast::statement_kind _kind;
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
