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
#include "rush/ast/node.hpp"
#include "rush/ast/scope.hpp"
#include "rush/ast/context.hpp"

#include <cassert>

namespace rush::ast {
   void node::attach(ast::context& context) {
      if (_context != nullptr) detach(*this);
      auto scope = ast::scope {};
      attached(scope, *(_context = &context));
   }

   void node::attach(ast::scope& scope, ast::node& child) {
      child._context = _context;
      child.attached(scope, *child._context);
   }

   void node::detach(ast::node& child) {
      child.detached(*child._context);
      child._context = nullptr;
   }

   void node::adopt(ast::node& child, ast::node* parent) {
      // assert(child._parent == nullptr);
      child._parent = parent != nullptr ? parent : this;
      child.adopted(*child._parent);
   }

   void node::orphan(ast::node& child) {
      assert(child._parent == this);
      child.orphaned(*child._parent);
      child._parent = nullptr;
   }
}
