#pragma once

#ifndef RUSH_SEMA_SYMBOL_HPP
#define RUSH_SEMA_SYMBOL_HPP

#include "rush/sema/attributes.hpp"

#include <string>

namespace rush::sema {
	class symbol {
	public:
		symbol(std::string name)
			: _name(name) {}

		symbol(symbol&&) = default;
		symbol(symbol const&) = default;

		symbol& operator = (symbol&&) = default;
		symbol& operator = (symbol const&) = default;

		std::size_t id() const noexcept {
			return 0;
		}

		std::string name() const noexcept {
			return _name;
		}


		bool is_builtin() const noexcept {
			return true;
		}

		bool is_defined() const noexcept {
			return false;
		}

		bool has_access_modifier(access_modifier am) const noexcept {
			return has_flags(_flags, am);
		}

		bool is_type() const noexcept {
			return is_defined()
			&& has_flags(_flags, symbol_type::type);
		}

		bool is_function() const noexcept {
			return is_defined()
			&& has_flags(_flags, symbol_type::function);
		}

		bool is_constant() const noexcept {
			return is_defined()
			&& has_flags(_flags, symbol_type::constant);
		}

		bool is_variable() const noexcept {
			return is_defined()
			&& has_flags(_flags, symbol_type::variable);
		}

		symbol(std::string name, symbol_flag_t flags)
			: _scope(nullptr)
			, _name(name)
			, _flags(flags) {}

	private:
		scope* _scope;
		std::string _name;
		symbol_flag_t _flags;
	};

	inline bool operator == (symbol const& lhs, symbol const& rhs) {
		return lhs.name() == rhs.name();
	}

	inline bool operator != (symbol const& lhs, symbol const& rhs) {
		return !(lhs == rhs);
	}

	inline symbol make_type_symbol(std::string name) {
		return { name, make_flags(symbol_type::type) };
	}
	// inline symbol make_constant_symbol(std::string name)
	// inline symbol make_variable_symbol(std::string name);
	// inline symbol make_function_symbol(std::string name);
}

namespace std {
	template <>
	struct hash<rush::sema::symbol> {
		std::size_t operator ()(rush::sema::symbol const& s) const {
			std::hash<std::string> _hfn;
			return _hfn.operator()(s.name());
		}
	};
}

#endif // RUSH_SEMA_SYMBOL_HPP
