#pragma once

#ifndef RUSH_SEMA_SYMBOL_ENTRY_HPP
#define RUSH_SEMA_SYMBOL_ENTRY_HPP

#include "rush/sema/attributes.hpp"

#include <string>

namespace rush {
	class scope;
} // rush

namespace rush::sema {
	class symbol_entry {
		friend class ::rush::scope;

		symbol_entry(std::string name)
			: symbol_entry(std::move(name), 0) {}

	public:
		symbol_entry(std::string name, symbol_flag_t flags)
			: _name(name)
			, _flags(flags) {}

		std::string name() const noexcept { return _name; }
		symbol_flag_t flags() const noexcept { return _flags; }

	private:
		std::string _name;
		symbol_flag_t _flags;
	};

	inline bool operator == (symbol_entry const& lhs, symbol_entry const& rhs) { return lhs.name() == rhs.name(); }
	inline bool operator != (symbol_entry const& lhs, symbol_entry const& rhs) { return !(lhs == rhs); }

} // rush::sema


namespace std {
	template <>
	struct hash<rush::sema::symbol_entry> {
		std::size_t operator ()(rush::sema::symbol_entry const& s) const {
			std::hash<std::string> _hfn;
			return _hfn.operator()(s.name());
		}
	};
} // std


#endif // RUSH_SEMA_SYMBOL_ENTRY_HPP
