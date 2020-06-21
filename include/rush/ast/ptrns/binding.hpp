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

#ifndef RUSH_AST_PTRNS_BINDING_HPP
#define RUSH_AST_PTRNS_BINDING_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>

namespace rush::ast {
   enum class binding_kind : std::uint8_t {
      unknown,
      parameter,
      initializer,
      default_value,
   };

   class binding_pattern
      : public ast::pattern
      , public ast::expression {
   public:
      binding_pattern(
         std::unique_ptr<ast::pattern> ptrn,
         std::unique_ptr<ast::expression> expr)
         : _ptrn { std::move(ptrn) }
         , _expr { std::move(expr) }
         , _kind { binding_kind::unknown } {
            adopt(*_ptrn);
            adopt(*_expr);
         }

      binding_kind kind() const noexcept {
         return _kind == binding_kind::unknown
              ? _kind = resolve_binding_kind()
              : _kind;
      }

      virtual ast::type_ref result_type() const override {
         return _expr->result_type();
      }

      ast::pattern const& pattern() const noexcept {
         return *_ptrn;
      }

      ast::expression const& expression() const noexcept {
         return *_expr;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_binding_ptrn(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_ptrn);
         attach(scope, *_expr);
      }

      virtual void detached(ast::context&) override {
         detach(*_ptrn);
         detach(*_expr);
      }

   private:
      std::unique_ptr<ast::pattern> _ptrn;
      std::unique_ptr<ast::expression> _expr;
      mutable binding_kind _kind;

      binding_kind resolve_binding_kind() const;
   };
}

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::binding_pattern> binding(
      std::unique_ptr<ast::pattern> lhs,
      std::unique_ptr<ast::expression> rhs) {
         return std::make_unique<ast::binding_pattern>(
            std::move(lhs),
            std::move(rhs));
      }
}

#endif // RUSH_AST_PTRNS_BINDING_HPP
