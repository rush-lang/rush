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
#include "rush/ast/types.hpp"
#include "rush/ast/visitor.hpp"

namespace ast = rush::ast;
namespace types = rush::ast::types;

class type_matcher : public ast::visitor {
public:
   type_matcher(ast::type_ref type)
      : _type { std::move(type) }
      , _result { false } {}

   bool result() const noexcept {
      return _result;
   }

   virtual void visit_builtin_integral_type(ast::builtin_integral_type const& second) override {
      auto first = _type.as<ast::builtin_integral_type>();
      if (!first) return;
      _result =
         first->unit() == second.unit() &&
         first->size() == second.size() &&
         first->bit_width() == second.bit_width() &&
         first->is_signed() == second.is_signed();
   }

private:
   ast::type_ref _type;
   bool _result;
};

namespace rush::ast::types {
   bool match(ast::type_ref lhs, ast::type_ref rhs) {
      return (lhs == rhs) || rush::visit(lhs, type_matcher { rhs }).result();
   }
}
