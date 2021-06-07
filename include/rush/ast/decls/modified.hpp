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

#ifndef RUSH_AST_DECLS_MODIFIED_HPP
#define RUSH_AST_DECLS_MODIFIED_HPP

#include "rush/ast/decls/declaration.hpp"

#include <memory>

namespace rush::ast {

   enum class storage_modifier : std::uint8_t {
      auto_,
      static_,
      virtual_,
      abstract_,
      override_
   };

   class modified_declaration : public virtual ast::declaration {
   public:
      explicit modified_declaration(
         std::unique_ptr<ast::declaration> decl,
         ast::storage_modifier mod)
         : _decl { std::move(decl) }
         , _mod { mod } { adopt(*_decl); }

      virtual declaration_kind kind() const noexcept override {
         return _decl->kind();
      }

      ast::storage_modifier modifier() const noexcept {
         return _mod;
      }

      ast::declaration const& declaration() const noexcept {
         return *_decl;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_modified_decl(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_decl);
      }

      virtual void detached(ast::context&) override {
         detach(*_decl);
      }

   private:
      std::unique_ptr<ast::declaration> _decl;
      ast::storage_modifier _mod;
   };

   namespace decls {
      inline std::unique_ptr<ast::modified_declaration> static_(std::unique_ptr<ast::declaration> decl) {
         return std::make_unique<ast::modified_declaration>(std::move(decl), storage_modifier::static_);
      }

      inline std::unique_ptr<ast::modified_declaration> virtual_(std::unique_ptr<ast::declaration> decl) {
         return std::make_unique<ast::modified_declaration>(std::move(decl), storage_modifier::virtual_);
      }

      inline std::unique_ptr<ast::modified_declaration> abstract_(std::unique_ptr<ast::declaration> decl) {
         return std::make_unique<ast::modified_declaration>(std::move(decl), storage_modifier::abstract_);
      }

      inline std::unique_ptr<ast::modified_declaration> override_(std::unique_ptr<ast::declaration> decl) {
         return std::make_unique<ast::modified_declaration>(std::move(decl), storage_modifier::override_);
      }
   }
} // rush::ast

#endif // RUSH_AST_DECLS_MODIFIED_HPP
