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

#ifndef RUSH_AST_STMTS_LOOP_HPP
#define RUSH_AST_STMTS_LOOP_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/ptrns/discard.hpp"
#include "rush/ast/decls/constant.hpp"

#include <memory>

namespace rush::ast {
   class iteration_statement;

   namespace stmts {
      std::unique_ptr<iteration_statement> for_(
			std::unique_ptr<ast::pattern> ptrn,
			std::unique_ptr<ast::expression> expr,
			std::unique_ptr<ast::statement> then);
   }

   class iteration_statement : public statement {
      struct factory_tag_t {};

      friend std::unique_ptr<iteration_statement> stmts::for_(
			std::unique_ptr<ast::pattern> ptrn,
			std::unique_ptr<ast::expression> expr,
			std::unique_ptr<ast::statement> then);

   public:
      iteration_statement(
         ast::statement_kind kind,
         std::unique_ptr<ast::pattern> ptrn,
         std::unique_ptr<ast::expression> expr,
         std::unique_ptr<ast::statement> body,
         factory_tag_t)
         : statement { kind }
         , _decl { decls::constant(std::move(ptrn)) }
         , _expr { std::move(expr) }
         , _body { std::move(body) } {}

      ast::pattern const& pattern() const noexcept {
         return _decl->pattern();
      }

      ast::expression const& expression() const noexcept {
         return *_expr;
      }

      ast::statement const& body() const noexcept {
         return *_body;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_iteration_stmt(*this);
      }

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         attach(*_decl);
         attach(*_body);
      }

      virtual void detached(ast::node*, ast::context&) override {
         detach(*_decl);
         detach(*_body);
      }

   private:
      std::unique_ptr<ast::storage_declaration> _decl;
      std::unique_ptr<ast::expression> _expr;
		std::unique_ptr<ast::statement> _body;
   };
} // rush::ast


namespace rush::ast::stmts {
   inline std::unique_ptr<iteration_statement> for_(
      std::unique_ptr<ast::pattern> ptrn,
      std::unique_ptr<ast::expression> expr,
      std::unique_ptr<ast::statement> then) {
         return std::make_unique<iteration_statement>(
            statement_kind::for_,
            std::move(ptrn),
            std::move(expr),
            std::move(then),
            iteration_statement::factory_tag_t {});
      }

   inline std::unique_ptr<iteration_statement> for_(
      std::unique_ptr<ast::expression> expr,
      std::unique_ptr<ast::statement> then) {
         return for_(
            ptrns::discard(),
            std::move(expr),
            std::move(then));
      }
} // rush::ast::stmts

#endif // RUSH_AST_STMTS_LOOP_HPP
