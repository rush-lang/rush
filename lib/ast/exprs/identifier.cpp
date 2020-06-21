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
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/scope.hpp"

namespace rush::ast {
   void identifier_expression::attached(ast::scope& scope, ast::context& context) {
      auto& sym = scope.lookup(_name);
      _decl = sym.is_undefined()
            ? context.undeclared_declaration(_name)
            : sym.is_overloaded()
            ? context.overloaded_declaration(_name, sym.begin(), sym.end())
            : &(**sym.begin());
   }
   void identifier_expression::detached(ast::context&) {
      _decl = nullptr;
   }
} // rush::ast
