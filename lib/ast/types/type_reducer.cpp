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
#include "rush/ast/types/type_matcher.hpp"
#include "rush/ast/types/type_reducer.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/types.hpp"

namespace ast = rush::ast;
namespace types = rush::ast::types;

class type_reducer : public ast::visitor {
private:
   ast::type_ref _other;
   ast::type_ref _result;

public:
   type_reducer(ast::type_ref other)
      : _other { other }
      , _result { ast::types::irreducible } {}

   ast::type_ref result() { return _result; }

   // virtual void visit_type_extension(ast::type_extension const& type) {};
   // virtual void visit_builtin_error_type(ast::builtin_error_type const& type) {};
   // virtual void visit_builtin_void_type(ast::builtin_void_type const& type) {};
   // virtual void visit_builtin_bool_type(ast::builtin_bool_type const& type) {};

   virtual void visit_builtin_integral_type(ast::builtin_integral_type const& type) {
      if (auto other = _other.as<ast::builtin_integral_type>()) {
         _result = type.bit_width() < other->bit_width() ? *other : type;
      } else if (auto other = _other.as<ast::builtin_floating_point_type>()) {
         _result = type.size() <= other->size() ? *other : _result;
         // todo?: add support for f(long, float) = double
         //        when context is added to the reducer.
      }
   }

   virtual void visit_builtin_floating_type(ast::builtin_floating_point_type const& type) {
      if (auto other = _other.as<ast::builtin_floating_point_type>()) {
         _result = type.size() < other->size() ? *other : type;
      }
   };

   virtual void visit_builtin_string_type(ast::builtin_string_type const& type) {};

   // virtual void visit_array_type(ast::array_type const& type) {};
   // virtual void visit_tuple_type(ast::tuple_type const& type) {};
   // virtual void visit_enum_type(ast::enum_type const& type) {};
   virtual void visit_class_type(ast::class_type const& type) {};
   virtual void visit_struct_type(ast::struct_type const& type) {};
   virtual void visit_concept_type(ast::concept_type const& type) {};
   virtual void visit_interface_type(ast::interface_type const& type) {};
   // virtual void visit_function_type(ast::function_type const& type) {};

   virtual void visit_optional_type(ast::optional_type const& type) {
      if (type.underlying_type() == _other) {
         _result = type;
      } else {
         auto result = rush::visit(type.underlying_type(), *this).result();
         if (type.underlying_type() == result) _result = type;
         else {
            // todo: get optional type for result which will be the common type
            //       between type and _other.
         }
      }
   };
   virtual void visit_user_type(ast::user_type const& type) {};
};

namespace rush::ast::types {
   ast::type_ref reduce(ast::type_ref lhs, ast::type_ref rhs) {
      if (lhs.kind() == ast::type_kind::error
       || rhs.kind() == ast::type_kind::error)
         return ast::types::irreducible;

      if (types::match(lhs, rhs))
         return lhs;

      auto result = rush::visit(lhs, type_reducer { rhs }).result();
      return result == ast::types::irreducible
           ? rush::visit(rhs, type_reducer { lhs }).result()
           : result;
   }
}
