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
   enum class declaration_kind : std::uint8_t;

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

      //! \brief Constructs an instance of an identifier with specified resolver.
      //!        The identifier will also subscribe to the resolver.
      identifier(resolver& res);

      //! \brief Move constructor, when passed an unresolved identifier this identifier will subscribe to the same resolver.
      identifier(identifier&& other);

      //! \brief Move assignment, when passed an unresolved identifier this identifier will subscribe to the same resolver.
      void operator = (identifier&& other);

      //! \brief Returns true if the declaration for this identifier hasn't been resolved yet.
      bool is_unresolved() const noexcept;

      //! \brief Returns the name of the identifier.
      std::string name() const noexcept;

      //! \brief Returns the type of identifier as specified by the declaration if resolved; otherwise an error type.
      ast::type_ref type() const noexcept;

      //! \brief Returns the kind of declaration this identifier refers to.
      ast::declaration_kind kind() const noexcept;

      //! \brief Returns the declaration for a resolved identifier.
      //!        Note: calling this function before the identifier has been resolved is an error.
      //!              The caller should first check is_unresolved before calling this function.
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
            friend class identifier_resolver;
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
            std::for_each(_cbs.begin(), _cbs.end(),
               [&decl](auto& cb) { cb.operator()(decl); });
         }

      private:
         std::vector<resolver_callback_t> _cbs;
         std::string _name;

         void listen(identifier* id) {
            _cbs.emplace_back(resolver_callback_t { id });
         }

         void replace_listener(identifier* orig, identifier* id) {
            std::replace_if(
               _cbs.begin(), _cbs.end(),
               [&orig](auto& elem) { return elem._ident == orig; },
               resolver_callback_t { id });
         }
      };
   }
}

#endif // RUSH_AST_IDENTIFIER_HPP
