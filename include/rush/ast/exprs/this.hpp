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

#ifndef RUSH_AST_EXPRS_THIS_HPP
#define RUSH_AST_EXPRS_THIS_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/type.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/iterator.hpp"

#include <memory>

namespace rush::ast {
   class this_expression : public ast::expression {
   public:
      this_expression() = default;

      ast::type_declaration const* declaration() const noexcept {
         return _decl;
      }

      virtual ast::type_ref result_type() const override {
         return declaration()
              ? declaration()->type()
              : ast::types::undefined;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_this_expr(*this);
      };

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         auto it = ast::find_ancestor<ast::type_declaration>(this);
         if (it != ast::ancestor_iterator()) _decl = &*it;
      }

      virtual void detached(ast::node*, ast::context&) override {
         _decl = nullptr;
      }

   private:
      ast::type_declaration const* _decl;
   };
}

namespace rush::ast::exprs {
   inline std::unique_ptr<ast::this_expression> this_() {
      return std::make_unique<ast::this_expression>();
   }

   inline std::unique_ptr<ast::this_expression> base_() {
      return std::make_unique<ast::this_expression>();
   }
}

#endif // RUSH_AST_EXPRS_THIS_HPP
