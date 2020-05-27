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
      ast::pattern& pattern() const noexcept {
         return *_patt;
      }

   protected:
      storage_declaration(std::unique_ptr<ast::pattern> patt)
         : _patt { std::move(patt) } {}

      virtual void attached(ast::node*, ast::context&) override {
         attach(*_patt);
      }

      virtual void detached(ast::node*, ast::context&) override {
         detach(*_patt);
      }

   private:
      std::unique_ptr<ast::pattern> _patt;
   };
} // rush::ast


#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

#endif // RUSH_AST_DECLS_STORAGE_HPP
