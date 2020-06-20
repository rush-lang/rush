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

#ifndef RUSH_AST_EXPRS_NEW_HPP
#define RUSH_AST_EXPRS_NEW_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/exprs/expression.hpp"

#include <memory>

namespace rush::ast {
   class new_expression : public ast::expression {
   public:
      new_expression(std::unique_ptr<ast::expression> expr)
         : _expr { std::move(expr) } {}

      virtual ast::type_ref result_type() const override {
         return _expr->result_type();
      }

      ast::expression const& expression() const noexcept {
         return *_expr;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_new_expr(*this);
      };

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_expr);
      };

      virtual void detached(ast::context&) override {
         detach(*_expr);
      };

   private:
      std::unique_ptr<ast::expression> _expr;
   };
}

namespace rush::ast::exprs {
   inline std::unique_ptr<ast::new_expression> new_(std::unique_ptr<ast::expression> expr) {
      return std::make_unique<ast::new_expression>(std::move(expr));
   }
}

#endif // RUSH_AST_EXPRS_NEW_HPP
