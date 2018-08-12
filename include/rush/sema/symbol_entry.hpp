#pragma once

#ifndef RUSH_SEMA_SYMBOL_ENTRY_HPP
#define RUSH_SEMA_SYMBOL_ENTRY_HPP

#include "rush/sema/attributes.hpp"
#include "rush/sema/global_scope.hpp"
#include "rush/sema/symbol.hpp"

#include <string>
#include <optional>

namespace rush::sema {
	class symbol_entry final {
		friend class ::rush::scope;
		friend class ::rush::sema::symbol;
		friend symbol_entry make_entry(std::string, symbol_attributes_t);
		friend symbol_entry make_entry(std::string, symbol const&, symbol_attributes_t);

		symbol_entry(std::string name, symbol_attributes_t attrs = 0)
			: symbol_entry(std::move(name), {}, attrs) {}

		symbol_entry(std::string name, symbol const& type, symbol_attributes_t attrs)
			: _type(type)
			, _name(std::move(name))
			, _flags(attrs) {}

	public:
		symbol type() const noexcept { return _type; }
		std::string name() const noexcept { return _name; }
		symbol_attributes_t flags() const noexcept { return _flags; }

	private:
		symbol _type;
		std::string _name;
		symbol_attributes_t _flags;
	};

	inline bool operator == (symbol_entry const& lhs, symbol_entry const& rhs) { return lhs.name() == rhs.name(); }
	inline bool operator != (symbol_entry const& lhs, symbol_entry const& rhs) { return !(lhs == rhs); }

	symbol_entry make_type_entry(std::string name, access_modifier);
	symbol_entry make_function_entry(std::string name, symbol const& return_type, access_modifier = access_modifier::internal);
	symbol_entry make_variable_entry(std::string name, symbol const& type, storage_class_specifier = storage_class_specifier::auto_);
	symbol_entry make_constant_entry(std::string name, symbol const& type, storage_class_specifier = storage_class_specifier::auto_);
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
