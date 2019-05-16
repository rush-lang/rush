#pragma once

#ifndef RUSH_SEMA_SYMBOL_HPP
#define RUSH_SEMA_SYMBOL_HPP

#include "rush/sema/attributes.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <string>

namespace rush::sema {

   /*! \brief A value type that points to a declaration in an AST.
	 *         This type tracks declarations within lexical scopes
    *         and is returned on lookup during parsing.
	 */
	class symbol {
   public:
      static const symbol undefined;

      // rush::scope const& scope() const noexcept;

      std::string name() const noexcept {
         return !is_undefined()
            ? "" //_decl->name()
            : "<undefined>";
      }

      // symbol kind
		symbol_kind kind() const noexcept;

      // symbol kind convenience members.
		bool is_constant() const noexcept {
         return _decl->kind() == ast::declaration_kind::constant;
      }

		bool is_variable() const noexcept;
		bool is_function() const noexcept;
      bool is_struct() const noexcept;
      bool is_class() const noexcept;
      bool is_enum() const noexcept;
      bool is_concept() const noexcept;
      bool is_interface() const noexcept;
		bool is_undefined() const noexcept;

      // access modifiers
		access_modifier access_modifier() const noexcept;

		// access modifier convenience members.
		bool is_internal() const noexcept;
		bool is_exported() const noexcept;
		bool is_public() const noexcept;
		bool is_private() const noexcept;
		bool is_protected() const noexcept;

      // storage class specifiers
		storage_class_specifier storage_duration() const noexcept;

		// convenience members
		bool is_auto_storage_duration() const noexcept;
		bool is_static_storage_duration() const noexcept;
		bool is_thread_local_storage_duration() const noexcept;

   private:
      ast::declaration* _decl;
   };

   // inline bool operator == (symbol const& lhs, symbol const& rhs) {
	// 	return lhs.id() == rhs.id();
	// }

	// inline bool operator != (symbol const& lhs, symbol const& rhs) {
	// 	return !(lhs == rhs);
	// }

	// extern symbol const& symbol_undefined;
}

#endif // RUSH_SEMA_SYMBOL_HPP
