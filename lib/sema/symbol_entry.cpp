#include "rush/sema/symbol_entry.hpp"

namespace rush::sema {
	inline symbol_attributes_t make_attributes() {
		return 0;
	}

	inline symbol_attributes_t make_attributes(symbol_kind st) {
		return (static_cast<symbol_attributes_t>(st) & 0x07);
	}

	inline symbol_attributes_t make_attributes(access_modifier am) {
		return (static_cast<symbol_attributes_t>(am) & 0x07) << 3;
	}

	inline symbol_attributes_t make_attributes(storage_class_specifier scs) {
		return (static_cast<symbol_attributes_t>(scs) & 0x03) << 6;
	}

	template <typename Flag, typename... Args>
	inline symbol_attributes_t make_attributes(Flag first, Args... rest) {
		return make_attributes(first) | make_attributes(rest...);
	}

	symbol_entry make_entry(std::string name, symbol_attributes_t attrs) {
		return { std::move(name), attrs };
	}

	symbol_entry make_entry(std::string name, symbol const& type, symbol_attributes_t attrs) {
		return { std::move(name), type, attrs };
	}

	symbol_entry make_undefined_entry(std::string name) {
		return make_entry(std::move(name), make_attributes(symbol_kind::undefined));
	}

	symbol_entry make_type_entry(std::string name, access_modifier am) {
		return make_entry(std::move(name), make_attributes(symbol_kind::type, am));
	}

	symbol_entry make_function_entry(std::string name, symbol const& return_type, access_modifier am) {
		return make_entry(std::move(name), return_type, make_attributes(symbol_kind::function, am));
	}

	symbol_entry make_variable_entry(std::string name, symbol const& type, storage_class_specifier scs) {
		return make_entry(std::move(name), type, make_attributes(symbol_kind::variable, scs));
	}

	symbol_entry make_constant_entry(std::string name, symbol const& type, storage_class_specifier scs) {
		return make_entry(std::move(name), type, make_attributes(symbol_kind::constant, scs));
	}
} // rush::sema