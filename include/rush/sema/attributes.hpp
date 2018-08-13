#pragma once

#ifndef RUSH_SEMA_ATTRIBUTES_HPP
#define RUSH_SEMA_ATTRIBUTES_HPP

#include <cstdint>

namespace rush::sema {

	// bits 1-3: symbol_kind
	// bits 4-7: access_modifier
	// bits 8-9: storage_class_specifier
	using symbol_attributes_t = std::uint32_t;

	// \brief An enumeration of mutually exclusive symbol types.
	enum class symbol_kind : std::uint8_t {
		undefined,
		variable,
		constant,
		function,
		type,
	};

	// \brief An enumeration of mutually exclusive access modifiers.
	enum class access_modifier : std::uint8_t {
		internal,
		exported,
		public_,
		private_,
		protected_,
	};

	// \brief An enumeration of mutually exclusive storage class specifiers.
	enum class storage_class_specifier : std::uint8_t {
		auto_,
		static_,
		thread_local_
	};

	inline symbol_attributes_t make_attributes() {
		return 0;
	}

	inline symbol_attributes_t make_attributes(symbol_kind st) {
		return (static_cast<symbol_attributes_t>(st) & 0x07);
	};

	inline symbol_attributes_t make_attributes(access_modifier am) {
		return (static_cast<symbol_attributes_t>(am) & 0x07) << 3;
	};

	inline symbol_attributes_t make_attributes(storage_class_specifier scs) {
		return (static_cast<symbol_attributes_t>(scs) & 0x03) << 6;
	}

	template <typename Flag, typename... Args>
	symbol_attributes_t make_attributes(Flag first, Args... rest) {
		return make_attributes(first) | make_attributes(rest...);
	}

	inline bool has_attributes(symbol_attributes_t) {
		return true;
	}

	inline bool has_attributes(symbol_attributes_t flags, symbol_kind st) {
		auto val = static_cast<symbol_attributes_t>(st);
		return (flags & 0x07) == val;
	}

	inline bool has_attributes(symbol_attributes_t flags, access_modifier am) {
		auto val = static_cast<symbol_attributes_t>(am);
		return ((flags >> 3) & 0x07) == val;
	}

	inline bool has_attributes(symbol_attributes_t flags, storage_class_specifier scs) {
		auto val = static_cast<symbol_attributes_t>(scs);
		return ((flags >> 6) & 0x03) == val;
	}

	template <typename Flag, typename... Args>
	bool has_attributes(symbol_attributes_t flags, Flag first, Args... rest) {
		return has_attributes(flags, first) && has_attributes(flags, rest...);
	}
} // rush::sema

#endif // RUSH_SEMA_ATTRIBUTES_HPP
