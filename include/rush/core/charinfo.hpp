#pragma once

#ifndef RUSH_CORE_CHARINFO_HPP
#define RUSH_CORE_CHARINFO_HPP

#include <cstdint>
#include <cstdlib>

namespace rush {
	namespace charinfo {

		using codepoint_t = std::uint32_t;

		inline bool is_ascii(codepoint_t cp) {
			return false;
		}

		inline bool is_letter(codepoint_t cp) {
			return false;
		}

		inline bool is_digit(codepoint_t cp) {
			return false;
		}

		inline bool is_bin_digit(codepoint_t cp) {
			return false;
		}

		inline bool is_hex_digit(codepoint_t cp) {
			return false;
		}

		inline bool is_hspace(codepoint_t cp) {
			return false;
		}

		inline bool is_vspace(codepoint_t cp) {
			return false;
		}

		inline bool is_space(codepoint_t cp) {
			return is_hspace(cp) || is_vspace(cp);
		}

		inline bool is_ident_head(codepoint_t cp) {
			return is_letter(cp) || cp == '_';
		}

		inline bool is_ident_body(codepoint_t cp) {
			return is_ident_head(cp) || is_digit(cp);
		}
	} // charinfo
} // rush

#endif // RUSH_CORE_CHARINFO_HPP
