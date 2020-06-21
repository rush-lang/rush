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
#include "rush/ast/decls/struct.hpp"
#include "rush/ast/scope.hpp"
#include "rush/ast/scope_inserter.hpp"

namespace rush::ast {
   void struct_declaration::attached(ast::scope& scope, ast::context&) {
      scope.insert(*this);
      scope.push(ast::scope_kind::struct_);
      std::for_each(_sections.begin(), _sections.end(), [this, &scope](auto& s) { rush::visit(*s, ast::scope_inserter { scope }); });
      std::for_each(_sections.begin(), _sections.end(), [this, &scope](auto& s) { attach(scope, *s); });
      scope.pop();
   };

   void struct_declaration::detached(ast::context&) {
      std::for_each(_sections.begin(), _sections.end(),
         [this](auto& s) { detach(*s); });
   };
} // rush::ast
