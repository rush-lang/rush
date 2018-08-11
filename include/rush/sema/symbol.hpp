#pragma once

#ifndef RUSH_SEMA_SYMBOL_HPP
#define RUSH_SEMA_SYMBOL_HPP

#include "rush/sema/attributes.hpp"

#include <string>

namespace rush {
	class scope;
}

namespace rush::sema {
	class symbol_entry {
	public:
		symbol_entry(std::string name)
			: symbol_entry(std::move(name), 0) {}

		symbol_entry(std::string name, symbol_flag_t flags)
			: _name(name)
			, _flags(flags) {}

		std::string name() const noexcept { return _name; }
		symbol_flag_t flags() const noexcept { return _flags; }

	private:
		std::string _name;
		symbol_flag_t _flags;
	};

	class symbol {
		friend class rush::scope;

	public:
		symbol(symbol&&) = default;
		symbol(symbol const&) = default;

		symbol& operator = (symbol&&) = default;
		symbol& operator = (symbol const&) = default;

		std::size_t id() const noexcept;

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

	private:
		symbol(scope const& scope, std::string name, symbol_flag_t flags)
			: _scope(&scope)
			, _name(name)
			, _flags(flags) {}

		scope const* const _scope;
		std::string _name;
		symbol_flag_t _flags;
	};

	inline bool operator == (symbol const& lhs, symbol const& rhs) {
		return lhs.name() == rhs.name();
	}

	inline bool operator != (symbol const& lhs, symbol const& rhs) {
		return !(lhs == rhs);
	}

	inline symbol_entry make_type_symbol(std::string name) {
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
