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

#ifndef RUSH_AST_DECLS_CLASS_HPP
#define RUSH_AST_DECLS_CLASS_HPP

#include "rush/ast/decls/type.hpp"
#include "rush/ast/decls/member.hpp"
#include "rush/ast/context.hpp"

#include <string>
#include <string_view>

namespace rush::ast {

   class class_declaration : public ast::type_declaration {
   public:
      class_declaration(std::string name)
         : ast::type_declaration { std::move(name) } {}

		virtual declaration_kind kind() const noexcept override {
         return declaration_kind::class_;
      }

      std::vector<std::unique_ptr<ast::member_declaration>> const& members() const noexcept {
         return _members;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_class_decl(*this);
      };

   protected:
      virtual void attached(ast::node*, ast::context&) override {
      };

      virtual void detached(ast::node*, ast::context&) override {
      };

   private:
      std::vector<std::unique_ptr<ast::member_declaration>> _members;
   };
} // rush::ast

namespace rush::ast::decls {
   inline std::unique_ptr<class_declaration> class_(std::string name) {
      return std::make_unique<class_declaration>(
         std::move(name));
   }
}


#endif // RUSH_AST_DECLS_CLASS_HPP
