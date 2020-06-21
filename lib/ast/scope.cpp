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
#include "rush/ast/scope.hpp"
#include "rush/ast/traversal.hpp"

using namespace rush;

class scope_inserter_visitor : public ast::traversal {

};

namespace rush::ast {
   void scope_frame::insert(ast::nominal_declaration const& decl) {
      if (kind() == ast::scope_kind::pseudo)
         return;

      auto it = _symbols.find(decl.name());
      if (it != _symbols.end()) {
         it->second->insert(decl); // insert overload.
      } else {
         auto sym = std::make_unique<ast::symbol>(decl.name());
         sym->insert(decl);
         auto res = _symbols.insert({
            sym->name(),
            std::move(sym)
         });
      }
   }

   ast::symbol const& scope_frame::lookup(std::string_view name) const {
      if (kind() != ast::scope_kind::pseudo) {
         auto it = _symbols.find(name);
         if (it != _symbols.end())
            return *it->second;
      }

      if (auto p = parent())
         return p->lookup(name);

      auto sym = std::make_unique<ast::symbol>(name);
      auto res = _symbols.insert({
         sym->name(),
         std::move(sym)
      });

      return *res.first->second;
   }


   scope::scope() {
      _push_global();
   }

   void scope::push(scope_kind kind) {
      _frames.push({ kind, &_frames.top() });
   }

   void scope::pop() {
      _frames.pop();
      if (_frames.empty())
         _push_global();
   }

   void scope::insert(ast::nominal_declaration const& decl) {
      _frames.top().insert(decl);
   }

   ast::symbol const& scope::lookup(std::string_view name) const {
      return _frames.top().lookup(name);
   }

   void scope::_push_global() {
      _frames.push({ scope_kind::global, nullptr });
   }
} // rush::ast
