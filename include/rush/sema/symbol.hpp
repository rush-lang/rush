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

		bool is_builtin() const noexcept {
			return true;
		}

		bool is_defined() const noexcept {
			return false;
		}

		bool has_access_modifier(access_modifier am) const noexcept;
		bool is_type() const noexcept;
		bool is_function() const noexcept;
		bool is_constant() const noexcept;
		bool is_variable() const noexcept;

	private:
		rush::scope const* _scope;
		symbol_entry const* _entry;

		symbol() noexcept
			: _scope(nullptr)
			, _entry(nullptr) {}

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
}

#endif // RUSH_SEMA_SYMBOL_HPP
