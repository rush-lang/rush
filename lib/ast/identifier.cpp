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
#include "rush/ast/identifier.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/types/builtin.hpp"

#include <cassert>

using namespace rush;

struct identifier_name_visitor {
   std::string_view operator ()(ast::declaration const* decl) { return decl->name(); }
   std::string_view operator ()(ast::identifier::resolver* res) { return res->name(); }
};

struct identifier_type_visitor {
   ast::type_ref operator ()(ast::declaration const* decl) { return decl->type(); }
   ast::type_ref operator ()(ast::identifier::resolver* res) { return ast::types::undefined; }
};

struct identifier_kind_visitor {
   ast::declaration_kind operator ()(ast::declaration const* decl) { return decl->kind(); }
   ast::declaration_kind operator ()(ast::identifier::resolver* res) { return ast::declaration_kind::undefined; }
};

namespace rush::ast {

   identifier::identifier(resolver& res) : _val { &res } {
      res.listen(this);
   }

   identifier::identifier(identifier&& other) : _val { std::move(other._val) } {
      if (auto res = std::get_if<ast::identifier::resolver*>(&_val)) {
         (*res)->replace_listener(&other, this);
      }
   }

   void identifier::operator = (identifier&& other) {
      _val = std::move(other._val);
      if (auto res = std::get_if<ast::identifier::resolver*>(&_val)) {
         (*res)->replace_listener(&other, this);
      }
   }

   bool identifier::is_unresolved() const noexcept {
      return std::holds_alternative<ast::identifier::resolver*>(_val)
          || std::get<ast::declaration const*>(_val)->kind() == declaration_kind::undeclared;
   }

   std::string_view identifier::name() const noexcept {
      return std::visit(identifier_name_visitor {}, _val);
   }

   ast::type_ref identifier::type() const noexcept {
      return std::visit(identifier_type_visitor {}, _val);
   }

   ast::declaration_kind identifier::kind() const noexcept {
      return std::visit(identifier_kind_visitor {}, _val);
   }

   ast::declaration const& identifier::declaration() const noexcept {
      assert(!is_unresolved() && "attempting to access the declaration of an unresolved identifier.");
      return *std::get<ast::declaration const*>(_val);
   }
} // rush::ast
