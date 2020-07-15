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

#ifndef RUSH_AST_DECLS_STORAGE_HPP
#define RUSH_AST_DECLS_STORAGE_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/visitor.hpp"

#include "rush/extra/iterator_range.hpp"
#include "rush/ast/iterator.hpp"

namespace rush::ast {
   class storage_declaration : public declaration {
   public:
      ast::pattern& pattern() const noexcept {
         return *_ptrn;
      }

      bool is_mutable() const noexcept;
      bool is_readonly() const noexcept;

      auto names() const -> decltype(ast::iterator_range<ast::named_pattern>());
      auto annotations() const -> decltype(ast::iterator_range<ast::type_annotation_pattern>());
      auto initializers() const -> decltype(ast::iterator_range<ast::binding_pattern>());

   protected:
      storage_declaration(std::unique_ptr<ast::pattern> ptrn)
         : _ptrn { std::move(ptrn) } { adopt(*_ptrn); }

      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_ptrn);
      }

      virtual void detached(ast::context&) override {
         detach(*_ptrn);
      }

   private:
      std::unique_ptr<ast::pattern> _ptrn;
   };
} // rush::ast

#endif // RUSH_AST_DECLS_STORAGE_HPP
