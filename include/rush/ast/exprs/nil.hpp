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

#ifndef RUSH_AST_EXPRS_NIL_HPP
#define RUSH_AST_EXPRS_NIL_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/context.hpp"

#include <memory>

namespace rush::ast {
   class nil_expression : public ast::expression {
   public:
      nil_expression(ast::type_ref type)
         : _type { std::move(type) } {}

      virtual ast::type_ref result_type() const override {
         return _type == ast::types::undefined
              ? _type = resolve_type()
              : _type;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_nil_expr(*this);
      };

   protected:
      virtual void attached(ast::scope&, ast::context&) override {};
      virtual void detached(ast::context&) override {};

   private:
      mutable ast::type_ref _type;

      ast::type_ref resolve_type() const;
   };
}

namespace rush::ast::exprs {
   inline std::unique_ptr<ast::nil_expression> nil(ast::type_ref type) {
      return std::make_unique<ast::nil_expression>(std::move(type));
   }

   inline std::unique_ptr<ast::nil_expression> nil() {
      return nil(ast::types::undefined);
   }
}

#endif // RUSH_AST_EXPRS_NIL_HPP
