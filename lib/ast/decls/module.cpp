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
#include "rush/ast/decls/module.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/scope.hpp"
#include "rush/ast/scope_inserter.hpp"

namespace rush::ast {
   void module::attached(ast::scope& scope, ast::context&) {
      std::for_each(_imports.begin(), _imports.end(), [this, &scope](auto& imp) { attach(scope, *imp); });

      scope.push(ast::scope_kind::module);
      std::for_each(_decls.begin(), _decls.end(), [this, &scope](auto& decl) { rush::visit(*decl, ast::scope_inserter { scope }); });
      std::for_each(_decls.begin(), _decls.end(), [this, &scope](auto& decl) { attach(scope, *decl); });
      scope.pop();
   }

   void module::detached(ast::context&) {
      std::for_each(_imports.begin(), _imports.end(), [this](auto& imp) { detach(*imp); });
      std::for_each(_decls.begin(), _decls.end(), [this](auto& decl) { detach(*decl); });
   }
} // rush::ast
