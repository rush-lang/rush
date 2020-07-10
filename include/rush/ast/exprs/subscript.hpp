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

#ifndef RUSH_AST_EXPRS_SUBSCRIPT_HPP
#define RUSH_AST_EXPRS_SUBSCRIPT_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/types/builtin.hpp"

namespace rush::ast {
   class subscript_expression : public ast::expression {
   public:
      subscript_expression(
         std::unique_ptr<ast::expression> operand,
         std::unique_ptr<ast::expression> subscript)
         : _operand { std::move(operand) }
         , _subscript { std::move(subscript) } {
            adopt(*_operand);
            adopt(*_subscript);
         }

      ast::expression const& operand() const noexcept {
         return *_operand;
      }

      ast::expression const& subscript() const noexcept {
         return *_subscript;
      }

      virtual ast::type_ref result_type() const override {
         if (auto type = _operand->result_type().as<ast::array_type>()) {
            return type->underlying_type();
         }

         return ast::types::undefined;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         return v.visit_subscript_expr(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_operand);
         attach(scope, *_subscript);
      }

      virtual void detached(ast::context&) override {
         detach(*_operand);
         detach(*_subscript);
      }

   private:
      std::unique_ptr<ast::expression> _operand;
      std::unique_ptr<ast::expression> _subscript;
   };


   namespace exprs {
      inline std::unique_ptr<ast::subscript_expression> subscript(
         std::unique_ptr<ast::expression> operand,
         std::unique_ptr<ast::expression> sub) {
         return std::make_unique<ast::subscript_expression>(
            std::move(operand),
            std::move(sub));
      }
   } // rush::ast::exprs
} // rush::ast



#endif // RUSH_AST_EXPRS_SUBSCRIPT_HPP
