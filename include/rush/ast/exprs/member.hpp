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

#ifndef RUSH_AST_EXPRS_MEMBER_HPP
#define RUSH_AST_EXPRS_MEMBER_HPP

#include "rush/ast/exprs/expression.hpp"

namespace rush::ast {
   class member_access_expression : public ast::expression {
   public:
      member_access_expression(
         std::unique_ptr<ast::expression> expr,
         std::unique_ptr<ast::expression> member)
         : _expr { std::move(expr) }
         , _member { std::move(member) } {}

      bool is_unresolved() const noexcept {
         return true;
      }

      ast::expression const& expression() const noexcept {
         return *_expr;
      }

      ast::expression const& member() const noexcept {
         return *_member;
      }

      virtual ast::type_ref result_type() const override {
         return member().result_type();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         return v.visit_member_access_expr(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_expr);
         attach(scope, *_member);
      }

      virtual void detached(ast::context&) override {
         detach(*_expr);
         detach(*_member);
      }

   private:
      std::unique_ptr<ast::expression> _expr;
      std::unique_ptr<ast::expression> _member;
   };

   namespace exprs {
      inline std::unique_ptr<member_access_expression> member_access(
         std::unique_ptr<expression> expr,
         std::unique_ptr<expression> member) {
            return std::make_unique<member_access_expression>(
               std::move(expr),
               std::move(member));
         }
   }
}

#endif // RUSH_AST_EXPRS_MEMBER_HPP
