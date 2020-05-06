#pragma once

#ifndef RUSH_PARSER_SCOPE_HPP
#define RUSH_PARSER_SCOPE_HPP

#include "rush/parser/symbol.hpp"

#include <unordered_map>
#include <stack>
#include <string>
#include <string_view>
#include <optional>
#include <memory>

namespace rush {
   class scope;
   class scope_chain;

   enum class scope_kind {
      global,
      function,
      struct_,
      class_,
      enum_,
      concept,
      interface,
      extension,
      module,
      block,
   };

   /*! \brief Represents a lexical scope of a particular kind (e.g. block, function, class).
    *         Each scope maintains a table of symbols and a pointer to its parent scope which
    *         enables a chained lookup of symbols while isolating symbol lookup from
    *         unrelated scopes. */
	class scope final {
      friend class scope_chain;

      scope(scope const&) = delete;
		void operator = (scope const&) = delete;

		scope& operator = (scope&& other) {
         _kind = other._kind;
			_parent = other._parent;
			_symtab = std::move(other._symtab);
			return *this;
		}

   public:
      scope(scope&& other)
         : _symtab { std::move(other._symtab) }
         , _parent(other._parent)
         , _kind(other._kind) {}

      using symbol_t = rush::symbol;
      using resolver_t = ast::identifier::resolver;
      using symbol_table_t = std::unordered_map<std::string_view, symbol_t>;
      using resolver_table_t = std::unordered_map<std::string_view, std::unique_ptr<resolver_t>>;

      //! \brief Gets the kind of this scope.
      scope_kind kind() const noexcept {
         return _kind;
      }

      //! \brief Gets the depth of the scope relative to its ancestor scopes.
      std::size_t depth() const noexcept {
			return !is_global() ? parent()->depth() + 1 : 0;
		}

      //! \brief Gets the parent of this scope if it is nested; otherwise null.
		scope* const parent() const noexcept {
			return _parent;
		}

      //! \brief Returns true if this scope is the root (i.e. does not have a parent).
		bool is_global() const noexcept {
			return _parent == nullptr;
		}

      //! \brief Returns true if this scope is nested directly within the specified parent scope.
		bool is_local_to(scope const& parent) const noexcept {
         return _parent == &parent;
		}

      //! \brief Returns true if the scope is nested somewhere with the parents chain of scopes.
      bool is_descendent_of(scope const& parent) const noexcept {
         if (_parent == nullptr) return false;
         if (is_local_to(parent)) return true;
         return _parent->is_local_to(parent);
      }

      //! \brief Inserts a symbol within the scope's symbol table.
      bool insert(symbol_t);

      //! \brief Gets a resolver for the given name. Later, when the declaration with the given
      //         name is added to the scope, the resolver is called with the declaration
      //         which is used to update identifiers.
      resolver_t& resolver(std::string_view name);

      //! \brief Gets all resolvers currently in play within the scope.
      std::vector<resolver_t*> resolvers();


      /*! \brief Performs a lookup of the symbol with the specified name.
       *         This is a chained lookup, starting at the scope the method
       *         is invoked on and then performing lookup against its the parent
       *         scope recursively until either the root/global scope has been reached
       *         or a symbol entry for the specified name is found.
       */
      symbol_t lookup(std::string_view name) const;

      /*! \brief Performs a lookup of the symbol with the specified name.
       *         This lookup is performed only within the scope the function is called on.
       *         Symbols in outer scopes will not be searched for.
       */
      symbol_t lookup_local(std::string_view name) const;

   private:
      symbol_table_t _symtab;
      resolver_table_t _restab;

      scope_kind _kind;
      scope* _parent;

      scope(scope_kind kind, scope* parent)
         : _symtab {}
         , _kind(kind)
         , _parent(parent) {}

      void hoist_resolvers();
	};


   /*! \brief Maintains a stack of lexical scopes during parsing and provides
    *         accessors for the current scope on top of the stack.
    */
   class scope_chain final {
      friend scope;

   public:
      using symbol_t = scope::symbol_t;
      using resolver_t = scope::resolver_t;

      scope_chain();

      //! \brief Returns a reference to the most deeply nested scope.
      scope const& current() const noexcept {
         return _scopes.top();
      }

      //! \brief Inserts a symbol within the current scope's symbol table.
      bool insert(symbol_t);

      //! \brief Gets all resolvers currently in play within the scope.
      std::vector<resolver_t*> resolvers();

      //! \brief Gets a resolver for the given name. Later, when the declaration with the given
      //         name is inserted, the resolver is called with the declaration inserted
      //         which is used to update identifiers referencing said declaration.
      resolver_t& resolver(std::string_view name);

      /*! \brief Pushes a new scope of scope_kind onto the end of the chain.
       *         The new scope will obtain the current scope as its parent.
       */
      void push(scope_kind);

      /*! \brief Pops the current scope from the chain.
       */
      void pop();

   private:
      std::stack<scope> _scopes;
   };
}

#endif // RUSH_PARSER_SCOPE_HPP
