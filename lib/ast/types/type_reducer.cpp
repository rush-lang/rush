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
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/type_matcher.hpp"
#include "rush/ast/types/type_reducer.hpp"
#include "rush/ast/visitor.hpp"

namespace ast = rush::ast;
namespace types = rush::ast::types;

class type_reducer {
public:
   type_reducer(ast::type_ref first)
      : _first { first } {}

   ast::type_ref operator ()(ast::type_ref second) const noexcept {
      if (_first.kind() == ast::type_kind::error
       || second.kind() == ast::type_kind::error)
         return ast::types::irreducible;

      if (types::match(_first, second))
         return _first;

      return ast::types::irreducible;
   }

private:
   ast::type_ref _first;
};

namespace rush::ast::types {
   ast::type_ref reduce(ast::type_ref lhs, ast::type_ref rhs) {
      auto reducer = type_reducer { lhs };
      return reducer(rhs);
   }
}
