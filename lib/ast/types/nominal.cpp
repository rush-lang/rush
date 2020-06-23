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
#include "rush/ast/types/nominal.hpp"
#include "rush/ast/decls/class.hpp"
#include "rush/ast/decls/struct.hpp"
#include "rush/ast/types.hpp"
#include "rush/ast/scope.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/visitor.hpp"

using namespace rush;

class named_type_resolver : public ast::visitor {
private:
   ast::scope& _scope;
   ast::context& _context;
   ast::type_ref _result;

public:
   named_type_resolver(ast::type_ref type, ast::scope& scope, ast::context& context)
      : _scope { scope }
      , _context { context }
      , _result { type } {}

   ast::type_ref result() {
      return _result;
   }

   virtual void visit_named_type(ast::named_type const& type) override {
      struct named_type_resolver : ast::visitor {
         ast::type_ref result;
         named_type_resolver(ast::named_type const& type) : result { type } {}
         virtual void visit_class_decl(ast::class_declaration const& decl) override { result = decl.type(); }
         virtual void visit_struct_decl(ast::struct_declaration const& decl) override { result = decl.type(); }
      };

      // TODO: lookup type declaration symbols only.
      auto sym = _scope.lookup(type.name());
      _result = sym.is_undefined()
              ? ast::types::undeclared
              : sym.is_overloaded()
              ? ast::types::ambiguous
              : rush::visit(**sym.begin(), named_type_resolver { type }).result;
   };

   virtual void visit_type_extension(ast::type_extension const& type) override {
      _result = _context.type_extension(
         rush::visit(type.underlying_type(), *this).result());
   }

   virtual void visit_array_type(ast::array_type const& type) override {
      _result = _context.array_type(
         rush::visit(type.underlying_type(), *this).result(),
         type.rank());
   }

   virtual void visit_tuple_type(ast::tuple_type const& type) override {
      auto types = type.types();
      std::vector<ast::type_ref> newtypes;
      std::transform(types.begin(), types.end(), std::back_inserter(newtypes),
         [this](auto& t) { return rush::visit(t, *this).result(); });
      _result = _context.tuple_type(newtypes);
   }

   virtual void visit_function_type(ast::function_type const& type) override {
      _result = _context.function_type(
         rush::visit(type.return_type(), *this).result(),
         rush::visit(type.parameter_types(), *this).result());
   }

   virtual void visit_optional_type(ast::optional_type const& type) override {
      _result = _context.optional_type(
         rush::visit(type.underlying_type(), *this).result());
   };

   virtual void visit_builtin_error_type(ast::builtin_error_type const& type) override { _result = type; };
   virtual void visit_builtin_void_type(ast::builtin_void_type const& type) override { _result = type; };
   virtual void visit_builtin_bool_type(ast::builtin_bool_type const& type) override { _result = type; };
   virtual void visit_builtin_integral_type(ast::builtin_integral_type const& type) override { _result = type; };
   virtual void visit_builtin_floating_type(ast::builtin_floating_point_type const& type) override { _result = type; };
   virtual void visit_builtin_string_type(ast::builtin_string_type const& type) override { _result = type; };
};

namespace rush::ast {
   ast::type_ref resolve_named_types(ast::type_ref type, ast::scope& scope, ast::context& context) {
      auto resolver = named_type_resolver { type, scope, context };
      return rush::visit(type, resolver).result();
   }
}
