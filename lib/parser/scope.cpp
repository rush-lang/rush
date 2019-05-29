#include "rush/parser/symbol.hpp"
#include "rush/parser/scope.hpp"

#include <iostream>

namespace rush {

   bool scope::insert(scope::symbol_t sym) {
      auto result = _symtab.insert({ sym.name(), sym });
      if (result.second) {
         auto it = _restab.find(sym.name());
         if (it != _restab.end())
            it->second->resolve(sym.declaration());
      };

      return result.second;
   }

   scope::resolver_t& scope::resolver(std::string const& name) {
      scope::resolver_table_t* restab;
      switch (kind()) {
      // in the case of function and block scopes we
      // should get or insert a resolver from/into the parent
      // because in this scenario we don't want to resolve declarations
      // that occur later in the same scope.
      // e.g.
      // let x = y
      // let y = 10
      // is an error when y is not defined in the outer scope.
      // all other scope kinds support cross resolution of declarations
      // within the same scope.
      default: break;
      case scope_kind::block:
      case scope_kind::function:
         return parent()->resolver(name);
      }

      auto it = _restab.find(name);
      if (it == _restab.end()) {
         auto result = _restab.insert({ name, std::make_unique<scope::resolver_t>(name) });
         return *result.first->second;
      }

      return *it->second;
   }

   scope::symbol_t scope::lookup(std::string name) const {
      auto sym = lookup_local(name);
      return sym.is_undefined() && !is_global()
         ? parent()->lookup(std::move(name))
         : sym;
   }

   scope::symbol_t scope::lookup_local(std::string name) const {
      auto it = _symtab.find(std::move(name));
		if (it != _symtab.end()) return it->second;
		return symbol::undefined;
   }


   void scope::hoist_resolvers() {
      // todo: move resolvers up the chain of scopes.
   }

   scope_chain::scope_chain() {
      _scopes.push({ scope_kind::global, nullptr });
   }

   bool scope_chain::insert(scope_chain::symbol_t sym) {
      return _scopes.top().insert(std::move(sym));
   }

   scope_chain::resolver_t& scope_chain::resolver(std::string const& name) {
      return _scopes.top().resolver(name);
   }

   void scope_chain::push(scope_kind kind) {
      // assert(!_scopes.empty() && "scope chain always contain a global scope")
      _scopes.push({ kind, &_scopes.top() });
   }

   void scope_chain::pop() {
      _scopes.top().hoist_resolvers();
      _scopes.pop();
      if (_scopes.empty()) {
         _scopes.push({ scope_kind::global, nullptr });
      }
   }
} // rush
