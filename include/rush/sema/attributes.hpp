#pragma once

#ifndef RUSH_SEMA_ATTRIBUTES_HPP
#define RUSH_SEMA_ATTRIBUTES_HPP

#include <cstdint>

namespace rush::sema {
	using symbol_flag_t = std::uint32_t;

	enum class symbol_type : std::uint8_t {
		undefined = 0x01,
		type = 0x02,
		variable = 0x04,
		constant = 0x08,
		function = 0x10,
	};

	enum class access_modifier : std::uint8_t {
		internal = 0x01,
		exported = 0x02,
		public_ = 0x04,
		private_ = 0x08,
		protected_ = 0x10,
	};

	inline symbol_flag_t make_flags() {
		return 0;
	}

	inline symbol_flag_t make_flags(symbol_type st) {
		return static_cast<symbol_flag_t>(st);
	};

	inline symbol_flag_t make_flags(access_modifier am) {
		return static_cast<symbol_flag_t>(am) << 8;
	};

	template <typename Flag, typename... Args>
	symbol_flag_t make_flags(Flag first, Args... rest) {
		return make_flags(first) | make_flags(rest...);
	}

	inline bool has_flags(symbol_flag_t) {
		return true;
	}

	inline bool has_flags(symbol_flag_t flags, symbol_type st) {
		auto val = static_cast<symbol_flag_t>(st);
		return val & flags;
	}

	inline bool has_flags(symbol_flag_t flags, access_modifier am) {
		auto val = static_cast<symbol_flag_t>(am);
		return (val & (flags >> 8));
	}

	template <typename Flag, typename... Args>
	bool has_flags(symbol_flag_t flags, Flag first, Args... rest) {
		return has_flags(flags, first) && has_flags(flags, rest...);
	}
} // rush::sema

#endif // RUSH_SEMA_ATTRIBUTES_HPP
