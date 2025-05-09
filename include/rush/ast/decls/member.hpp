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

#include "rush/extra/dereferencing_iterator.hpp"

#include "rush/ast/node.hpp"
#include "rush/ast/decls/access.hpp"
#include "rush/ast/decls/nominal.hpp"
#include "rush/ast/iterator.hpp"
#include "rush/ast/visitor.hpp"

#include <vector>
#include <string>
#include <cassert>
#include <algorithm>


namespace rush::ast {
   class member_declaration;
   class member_section_declaration;

   class member_declaration : public ast::nominal_declaration {
   public:
      ast::member_access access() const noexcept;

      bool is_public() const noexcept {
         return access() == ast::member_access::public_;
      }

      bool is_private() const noexcept {
         return access() == ast::member_access::private_;
      }

      bool is_protected() const noexcept {
         return access() == ast::member_access::protected_;
      }

      bool is_internal() const noexcept {
         return access() == ast::member_access::internal;
      }
   };

   class member_section_declaration : public declaration {
   private:
      ast::member_access _access;
      std::vector<std::unique_ptr<ast::member_declaration>> _members;

   public:
      member_section_declaration(
         ast::member_access acc,
         std::vector<std::unique_ptr<ast::member_declaration>> mems)
         : _access { acc }
         , _members { std::move(mems) } {
            std::for_each(_members.begin(), _members.end(),
               [this](auto& m) { adopt(*m); });
         }

      using member_iterator = decltype(rush::make_deref_iterator(_members.begin()));
      using member_const_iterator = decltype(rush::make_deref_iterator(_members.cbegin()));

      virtual ast::declaration_kind kind() const noexcept override {
         return ast::declaration_kind::undefined;
      }

      ast::member_access access() const noexcept {
         return _access;
      }

      auto empty() const noexcept {
         return _members.empty();
      }

      auto size() const noexcept {
         return _members.size();
      }

      auto& front() const noexcept {
         return *_members.front();
      }

      auto& back() const noexcept {
         return *_members.back();
      }

      auto begin() const noexcept {
         return rush::make_deref_iterator(_members.begin());
      }

      auto end() const noexcept {
         return rush::make_deref_iterator(_members.end());
      }

      ast::typed_node_iterator<ast::member_declaration> find(std::string_view name) const;

      rush::iterator_range<member_const_iterator> members() const noexcept {
         return rush::make_iterator_range(
            rush::make_deref_iterator(_members.begin()),
            rush::make_deref_iterator(_members.end()));
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_member_section_decl(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         std::for_each(_members.begin(), _members.end(),
            [this, &scope](auto& m) { attach(scope, *m); });
      }
      virtual void detached(ast::context&) override {
         std::for_each(_members.begin(), _members.end(),
            [this](auto& m) { detach(*m); });
      }
   };

   inline ast::member_access ast::member_declaration::access() const noexcept {
      auto sec = ast::find_ancestor<ast::member_section_declaration>(this);
      return sec != ast::ancestor_iterator()
           ? sec->access()
           : ast::member_access::internal;
   }
} // rush::ast

namespace rush::ast::decls {
   inline std::unique_ptr<ast::member_section_declaration> member_section(
      ast::member_access acc,
      std::vector<std::unique_ptr<ast::member_declaration>> members) {
         return std::make_unique<ast::member_section_declaration>(acc, std::move(members));
      }
}

#endif // RUSH_AST_DECLS_MEMBER_HPP
