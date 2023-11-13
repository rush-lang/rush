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

#ifndef RUSH_AST_DECLS_MODULE_HPP
#define RUSH_AST_DECLS_MODULE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/decls/access.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <vector>
#include <string>

namespace rush::ast {
   class declaration;

   class module_declaration : public ast::declaration {
   public:
      module_declaration(
         std::unique_ptr<ast::declaration> decl,
         ast::module_access acc)
      : _decl { std::move(decl) }
      , _access { acc }
      { adopt(*_decl); }

      ast::declaration const& declaration() const noexcept {
         return *_decl;
      }

		virtual declaration_kind kind() const override {
         return _decl->kind();
      }

      ast::module_access access() const noexcept {
         return _access;
      }

      bool is_extern() const noexcept {
         return false;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_module_decl(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_decl);
      }

      virtual void detached(ast::context&) override {
         detach(*_decl);
      }

   private:
      ast::module_access _access;
      std::unique_ptr<ast::declaration> _decl;
   };

   namespace decls {
      inline std::unique_ptr<ast::module_declaration> internal(std::unique_ptr<ast::declaration> decl) {
         return std::make_unique<ast::module_declaration>(std::move(decl), ast::module_access::internal);
      }

      inline std::unique_ptr<ast::module_declaration> exported(std::unique_ptr<ast::declaration> decl) {
         return std::make_unique<ast::module_declaration>(std::move(decl), ast::module_access::exported);
      }
   } // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_MODULE_HPP
