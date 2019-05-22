#include "rush/parser/symbol.hpp"
#include "rush/parser/scope.hpp"

#include <iostream>

namespace rush {

   void scope::insert(scope::symbol_t sym) {
      _symtab.insert({ sym.name(), sym });
   }

   scope::symbol_t scope::lookup(std::string name) const {
      auto sym = lookup_local(name);
      return sym.is_undefined()
         ? parent()->lookup(std::move(name))
         : sym;
   }

   scope::symbol_t scope::lookup_local(std::string name) const {
      auto it = _symtab.find(std::move(name));
		if (it != _symtab.end()) return it->second;
		return symbol::undefined;
   }


   scope_chain::scope_chain() {
      _scopes.push({ scope_kind::global, nullptr });
   }

   void scope_chain::insert(scope_chain::symbol_t sym) {
      _scopes.top().insert(std::move(sym));
   }

   void scope_chain::push(scope_kind kind) {
      // assert(!_scopes.empty() && "scope chain always contain a global scope")
      _scopes.push({ kind, &_scopes.top() });
   }

   void scope_chain::pop() {
      _scopes.pop();
      if (_scopes.empty()) {
         _scopes.push({ scope_kind::global, nullptr });
      }
   }
} // rush
