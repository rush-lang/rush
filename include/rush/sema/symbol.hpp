#pragma once

#ifndef RUSH_SEMA_SYMBOL_HPP
#define RUSH_SEMA_SYMBOL_HPP

#include "rush/sema/attributes.hpp"

#include <string>
#include <optional>

namespace rush {
	class scope;
}

namespace rush::sema {
	class symbol_entry;
	class symbol {
		friend class symbol_entry;
		friend class ::rush::scope;

	public:
		std::size_t id() const noexcept;
		std::string name() const noexcept;
		sema::symbol type() const noexcept;

		rush::scope const& scope() const noexcept;

		// symbol types
		symbol_kind symbol_kind() const noexcept;

		// convenience members
		bool is_type() const noexcept;
		bool is_function() const noexcept;
		bool is_constant() const noexcept;
		bool is_variable() const noexcept;
		bool is_undefined() const noexcept;

		// access modifiers
		access_modifier access_modifier() const noexcept;

		// convenience members
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
		static symbol_entry const undefined_entry;

		rush::scope const* _scope;
		symbol_entry const* _entry;

		symbol(rush::scope const& scope) noexcept
			: symbol(scope, undefined_entry) {}

		symbol(rush::scope const& scope, symbol_entry const& entry) noexcept
			: _scope(&scope)
			, _entry(&entry) {}
	};

	inline bool operator == (symbol const& lhs, symbol const& rhs) {
		return lhs.id() == rhs.id();
	}

	inline bool operator != (symbol const& lhs, symbol const& rhs) {
		return !(lhs == rhs);
	}

	extern symbol const& symbol_undefined;
}

#endif // RUSH_SEMA_SYMBOL_HPP
