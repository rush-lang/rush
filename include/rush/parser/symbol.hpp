#pragma once

#ifndef RUSH_PARSER_SYMBOL_HPP
#define RUSH_PARSER_SYMBOL_HPP

#include "rush/parser/attributes.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <string>

namespace rush {

   /*! \brief A value type that points to a declaration in an AST.
	 *         This type tracks declarations within lexical scopes
    *         and is returned on lookup during parsing.
	 */
	class symbol {
   public:
      static const symbol undefined;

      symbol()
         : _decl { nullptr } {}

      symbol(ast::declaration const& decl)
         : _decl { &decl } {}

      // rush::scope const& scope() const noexcept;

      std::string_view name() const noexcept {
         return !is_undefined()
            ? _decl->name()
            : "<undefined>";
      }

      ast::declaration const* declaration() const noexcept {
         return _decl;
      }

      // symbol kind
		rush::symbol_kind kind() const noexcept;

      // symbol kind convenience members.
		bool is_constant() const noexcept {
         return _decl->kind() == ast::declaration_kind::constant;
      }

		bool is_variable() const noexcept {
         return _decl->kind() == ast::declaration_kind::variable;
      }

		bool is_function() const noexcept {
         return _decl->kind() == ast::declaration_kind::function;
      }

      bool is_struct() const noexcept {
         return _decl->kind() == ast::declaration_kind::struct_;
      }

      bool is_class() const noexcept {
         return _decl->kind() == ast::declaration_kind::class_;
      }

      bool is_enum() const noexcept {
         return _decl->kind() == ast::declaration_kind::enum_;
      }

      bool is_concept() const noexcept {
         return _decl->kind() == ast::declaration_kind::concept;
      }

      bool is_interface() const noexcept {
         return _decl->kind() == ast::declaration_kind::interface;
      }

		bool is_undefined() const noexcept {
         return _decl == nullptr;
      }

      // access modifiers
		rush::access_modifier access_modifier() const noexcept;

		// access modifier convenience members.
		bool is_internal() const noexcept;
		bool is_exported() const noexcept;
		bool is_public() const noexcept;
		bool is_private() const noexcept;
		bool is_protected() const noexcept;

      // storage class specifiers
		rush::storage_class_specifier storage_duration() const noexcept;

		// convenience members
		bool is_auto_storage_duration() const noexcept;
		bool is_static_storage_duration() const noexcept;
		bool is_thread_local_storage_duration() const noexcept;

   private:
      ast::declaration const* _decl;
   };

   // inline bool operator == (symbol const& lhs, symbol const& rhs) {
	// 	return lhs.id() == rhs.id();
	// }

	// inline bool operator != (symbol const& lhs, symbol const& rhs) {
	// 	return !(lhs == rhs);
	// }

	// extern symbol const& symbol_undefined;
}

#endif // RUSH_PARSER_SYMBOL_HPP
