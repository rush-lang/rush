#include "rush/parser/symbol.hpp"
#include "rush/parser/scope.hpp"

#include <iostream>

namespace rush {

   bool scope::insert(scope::symbol_t sym) {
      auto result = _symtab.insert({ sym.name(), sym });
      if (result.second) {
         auto it = _restab.find(sym.name());
         if (it != _restab.end()) {
            it->second->resolve(sym.declaration());
            _restab.erase(it);
         }
      };

      return result.second;
   }

   scope::resolver_t& scope::resolver(std::string_view name) {
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
         return parent()->resolver(std::move(name));
      }

      auto it = _restab.find(name);
      if (it == _restab.end()) {
         auto res = std::make_unique<scope::resolver_t>(std::string { name });
         auto result = _restab.insert({ res->name(), std::move(res) });
         return *result.first->second;
      }

      return *it->second;
   }

   std::vector<scope::resolver_t*> scope::resolvers() {
      std::vector<scope::resolver_t*> results;
      std::transform(_restab.begin(), _restab.end(),
         std::back_inserter(results), [](auto& kv) { return kv.second.get(); });

      return std::move(results);
   }

   scope::symbol_t scope::lookup(std::string_view name) const {
      auto sym = lookup_local(name);
      return sym.is_undefined() && !is_global()
         ? parent()->lookup(std::move(name))
         : sym;
   }

   scope::symbol_t scope::lookup_local(std::string_view name) const {
      auto it = _symtab.find(std::move(name));
		if (it != _symtab.end()) return it->second;
		return symbol::undefined;
   }


   void scope::hoist_resolvers() {
      if (parent() != nullptr) {
         auto& restab = parent()->_restab;
         std::move(
            _restab.begin(),
            _restab.end(),
            std::inserter(restab, restab.begin()));
      }
   }

   scope_chain::scope_chain() {
      _scopes.push({ scope_kind::global, nullptr });
   }

   std::vector<scope_chain::resolver_t*> scope_chain::resolvers() {
      return _scopes.top().resolvers();
   }

   bool scope_chain::insert(scope_chain::symbol_t sym) {
      return _scopes.top().insert(std::move(sym));
   }

   scope_chain::resolver_t& scope_chain::resolver(std::string_view name) {
      return _scopes.top().resolver(name);
   }

   void scope_chain::push(scope_kind kind) {
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
