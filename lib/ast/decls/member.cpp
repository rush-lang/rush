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
#include "rush/ast/decls/member.hpp"
#include "rush/ast/decls/field.hpp"
#include "rush/ast/decls/method.hpp"
#include "rush/ast/decls/property.hpp"

namespace rush::ast {
   ast::typed_node_iterator<ast::member_declaration>
   member_section_declaration::find(std::string_view name) const {
      class find_member_by_name : public ast::visitor {
         std::string_view _name;
         bool _result;

      public:
         find_member_by_name(std::string_view name)
            : _name { std::move(name) }
            , _result { false } {}

         bool result() const noexcept {
            return _result;
         }

         virtual void visit_variable_field_decl(ast::variable_field_declaration const& decl) override {
            auto names = decl.names();
            auto it = std::find_if(names.begin(), names.end(),
               [this](auto& n) { return n.name() == _name; });
            _result = it != names.end();
         }

         virtual void visit_constant_field_decl(ast::constant_field_declaration const& decl) override {
            auto names = decl.names();
            auto it = std::find_if(names.begin(), names.end(),
               [this](auto& n) { return n.name() == _name; });
            _result = it != names.end();
         }

         virtual void visit_property_getter_decl(ast::property_getter_declaration const& decl) override {
            _result = decl.name() == _name;
         }

         virtual void visit_property_setter_decl(ast::property_setter_declaration const& decl) override {
            _result = decl.name() == _name;
         }

         virtual void visit_method_decl(ast::method_declaration const& decl) override {
            _result = decl.name() == _name;
         }
      };

      auto it = std::find_if(_members.begin(), _members.end(),
         [this, &name](auto& m) { return rush::visit(*m, find_member_by_name { name }).result(); });

      return it != _members.end()
         ? ast::iterator<ast::member_declaration>(it->get())
         : ast::iterator<ast::member_declaration>();
   }
} // rush::ast
