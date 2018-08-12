#pragma once

#ifndef RUSH_SEMA_SYMBOL_HPP
#define RUSH_SEMA_SYMBOL_HPP

#include "rush/sema/attributes.hpp"
#include "rush/sema/symbol_entry.hpp"

#include <string>

namespace rush {
	class scope;
}

namespace rush::sema {
	class symbol {
		friend class ::rush::scope;

	public:
		std::size_t id() const noexcept;
		std::string name() const noexcept;

		bool is_builtin() const noexcept {
			return true;
		}

		bool is_defined() const noexcept {
			return false;
		}

		bool has_access_modifier(access_modifier am) const noexcept {
			return has_flags(_entry->flags(), am);
		}

		bool is_type() const noexcept {
			return is_defined()
			&& has_flags(_entry->flags(), symbol_type::type);
		}

		bool is_function() const noexcept {
			return is_defined()
			&& has_flags(_entry->flags(), symbol_type::function);
		}

		bool is_constant() const noexcept {
			return is_defined()
			&& has_flags(_entry->flags(), symbol_type::constant);
		}

		bool is_variable() const noexcept {
			return is_defined()
			&& has_flags(_entry->flags(), symbol_type::variable);
		}

	private:
		symbol(scope const& scope, symbol_entry const& entry)
			: _scope(&scope)
			, _entry(&entry) {}

		scope const* _scope;
		symbol_entry const* _entry;
	};

	inline bool operator == (symbol const& lhs, symbol const& rhs) {
		return lhs.id() == rhs.id();
	}

	inline bool operator != (symbol const& lhs, symbol const& rhs) {
		return !(lhs == rhs);
	}
}

#endif // RUSH_SEMA_SYMBOL_HPP
