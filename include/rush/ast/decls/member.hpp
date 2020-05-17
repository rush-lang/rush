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

#ifndef RUSH_AST_DECLS_MEMBER_HPP
#define RUSH_AST_DECLS_MEMBER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/decls/access.hpp"
#include "rush/ast/decls/nominal.hpp"

#include <vector>
#include <string>

namespace rush::ast {
   class module;

   class member_declaration : public ast::nominal_declaration {
   public:
      member_declaration(
         ast::nominal_declaration const* owner,
         std::unique_ptr<ast::nominal_declaration> decl,
         ast::member_access acc)
      : _decl { std::move(decl) }
      , _owner { owner }
      , _access { acc } {}

      ast::declaration const& owner() const noexcept {
         return *_owner;
      }

      ast::declaration const& declaration() const noexcept {
         return *_decl;
      }

      virtual std::string_view name() const override {
         return _decl->name();
      }

      virtual ast::type_ref type() const override {
         return _decl->type();
      }

		virtual declaration_kind kind() const override {
         return _decl->kind();
      }

      ast::member_access access() const {
         return _access;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_member_decl(*this);
      }

      virtual void attach(ast::node& parent, ast::context& context) override {
         _decl->attach(parent, context);
      }

      virtual void detach(ast::node& parent, ast::context& context) override {
         _decl->detach(parent, context);
      }

   private:
      ast::member_access _access;
      ast::nominal_declaration const* const _owner;
      std::unique_ptr<ast::nominal_declaration> _decl;
   };
} // rush::ast

#endif // RUSH_AST_DECLS_MEMBER_HPP
