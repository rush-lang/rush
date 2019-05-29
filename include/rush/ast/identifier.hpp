#pragma once

#ifndef RUSH_AST_IDENTIFIER_HPP
#define RUSH_AST_IDENTIFIER_HPP

#include "rush/ast/types/type.hpp"

#include <string>
#include <variant>
#include <algorithm>
#include <vector>

namespace rush::ast {
   class declaration;

   namespace detail {
      class identifier_resolver;
   }

   class identifier {
      friend class declaration;
      friend class detail::identifier_resolver;

      identifier(identifier const&) = delete;
      void operator = (identifier const&) = delete;

   public:
      using resolver = detail::identifier_resolver;

      identifier(resolver& res);

      identifier(identifier&& other);
      void operator = (identifier&& other);

      bool is_unresolved() const noexcept;

      std::string name() const noexcept;

      ast::type_ref type() const noexcept;

      ast::declaration const& declaration() const noexcept;

   private:
      std::variant<
         ast::declaration const*,
         ast::identifier::resolver*> _val;

      identifier(ast::declaration const* decl)
         : _val { decl } {}
   };

   namespace detail {
      /*! \brief When parsing some identifiers may appear before the associated
      *         declaration has been parsed.
      *         In which case an identifier expression is created with a resolver callback
      *         to resolve the identifier at the moment the declaration is inserted
      *         into the scope. */
      class identifier_resolver {
         friend class ast::identifier;

         class resolver_callback_t {
         public:
            resolver_callback_t(ast::identifier* id)
               : _ident { id } {}

            void operator ()(ast::declaration const* decl) const {
               _ident->_val = decl;
            }

         private:
            identifier* _ident;
         };

      public:
         identifier_resolver(std::string name)
            : _name { std::move(name) } {}

         std::string const& name() const noexcept {
            return _name;
         }

         void resolve(ast::declaration const* decl) {
            std::for_each(_cbs.begin(), _cbs.end(), [&decl](auto& cb) { cb.operator()(decl); });
         }

      private:
         std::vector<resolver_callback_t> _cbs;
         std::string _name;

         void listen(identifier* id) {
            _cbs.emplace_back(resolver_callback_t { id });
         }

         void replace_listener(identifier* orig, identifier* id) {
            listen(id);
         }
      };
   }
}

#endif // RUSH_AST_IDENTIFIER_HPP
