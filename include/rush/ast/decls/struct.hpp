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

#ifndef RUSH_AST_DECLS_STRUCT_HPP
#define RUSH_AST_DECLS_STRUCT_HPP

#include "rush/extra/iterator_range.hpp"
#include "rush/extra/dereferencing_iterator.hpp"

#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/type_ref.hpp"

#include "rush/ast/decls/type.hpp"
#include "rush/ast/decls/member.hpp"

#include <string>
#include <string_view>

namespace rush::ast {
   class context;

   class struct_declaration
      : public ast::type_declaration {
   public:
      struct_declaration(
         std::string name,
         std::vector<std::unique_ptr<ast::member_section_declaration>> sections)
         : ast::type_declaration { std::move(name) }
         , _sections { std::move(sections) } {}

      virtual ast::declaration_kind kind() const noexcept override {
         return ast::declaration_kind::struct_;
      }

      ast::typed_node_iterator<ast::member_declaration> find(std::string_view name) const {
         for (auto& s : _sections) {
            auto it = s->find(std::move(name));
            if (it != ast::iterator()) return it;
         }

         return ast::iterator<ast::member_declaration>();
      }

      auto sections() const noexcept {
         return rush::make_iterator_range(
            rush::make_deref_iterator(_sections.begin()),
            rush::make_deref_iterator(_sections.end()));
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_struct_decl(*this);
      };

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         std::for_each(_sections.begin(), _sections.end(),
            [this, &scope](auto& s) { attach(scope, *s); });
      };

      virtual void detached(ast::context&) override {
         std::for_each(_sections.begin(), _sections.end(),
            [this](auto& s) { detach(*s); });
      };

   private:
      std::vector<std::unique_ptr<ast::member_section_declaration>> _sections;
   };
} // rush::ast

namespace rush::ast::decls {
   inline std::unique_ptr<ast::struct_declaration> struct_(
      std::string name,
      std::vector<std::unique_ptr<ast::member_section_declaration>> sections) {
      return std::make_unique<ast::struct_declaration>(
         std::move(name),
         std::move(sections));
   }
}


#endif // RUSH_AST_DECLS_STRUCT_HPP
