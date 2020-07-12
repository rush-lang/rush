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
   void module_node::attached(ast::scope& scope, ast::context& ctx) {
      scope.push(ast::scope_kind::module_);
      std::for_each(begin(), end(), [this, &scope](auto& src) {
         rush::visit(src, ast::scope_inserter { scope }); });
      ast::node_list<ast::source_node, ast::node>::attached(scope, ctx);
      scope.pop();
   }

   void module_node::detached(ast::context& ctx) {
      ast::node_list<ast::source_node, ast::node>::detached(ctx);
   }
} // rush::ast
