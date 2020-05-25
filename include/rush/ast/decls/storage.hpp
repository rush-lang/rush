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

#include "rush/ast/identifier.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/visitor.hpp"


namespace rush::ast {
   class storage_declaration : public declaration {
   public:
      virtual ast::type_ref type() const noexcept override {
         return resolve_type();
      }

      ast::pattern& pattern() const noexcept {
         return *_patt;
      }

      ast::expression* initializer() const noexcept {
         return _init.get();
      }

      bool is_initialized() const noexcept {
         return _init.operator bool();
      }

   protected:
      storage_declaration(
         std::unique_ptr<ast::pattern> patt,
         std::unique_ptr<ast::expression> init)
         : _patt { std::move(patt) }
         , _init { std::move(init) }
         , _type { ast::types::undefined } {}

      virtual void attached(ast::node*, ast::context&) override {
         if (_init) attach(*_init);
         attach(*_patt);
      }

      virtual void detached(ast::node*, ast::context&) override {
         detach(*_patt);
         if (_init) detach(*_init);
      }

   private:
      std::unique_ptr<ast::pattern> _patt;
      std::unique_ptr<ast::expression> _init;
      mutable ast::type_ref _type;
      mutable bool _resolving_type = false;

      ast::type_ref resolve_type() const {
         if (_resolving_type)
            return _type = ast::types::circular_ref;

         _resolving_type = true;
         auto result_type = (_type == ast::types::undefined && initializer())
            ? initializer()->result_type()
            : _type;
         _resolving_type = false;

         return _type = (result_type.kind() == type_kind::error)
              ? (_type != ast::types::circular_ref)
              ? ast::types::inference_fail
              : _type
              : result_type;
      }
   };
} // rush::ast


#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

#endif // RUSH_AST_DECLS_STORAGE_HPP
