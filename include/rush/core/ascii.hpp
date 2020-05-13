/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#pragma once

#ifndef RUSH_CORE_ASCII_HPP
#define RUSH_CORE_ASCII_HPP

#include <cstdint>
#include <cstdlib>

#include <cctype>

namespace rush {
	using codepoint_t = std::uint32_t;
	static const codepoint_t npos = -1;

	namespace ascii {
		constexpr bool is_ascii(codepoint_t cp) {
			return cp <= 127;
		}

		constexpr bool is_newline(codepoint_t cp) {
			return cp == '\n';
		}

		constexpr bool is_letter(codepoint_t cp) {
			// change to unicode codepoints.
			return ('a' <= cp && cp <= 'z')
				|| ('A' <= cp && cp <= 'Z');
		}

		constexpr bool is_digit(codepoint_t cp) {
			return '0' <= cp && cp <= '9';
		}

      constexpr bool is_alnum(codepoint_t cp) {
         return is_digit(cp) || is_letter(cp);
      }

		constexpr bool is_hex_digit(codepoint_t cp) {
			return is_digit(cp)
				|| ('a' <= cp && cp <= 'f')
				|| ('A' <= cp && cp <= 'F');
		}

		constexpr bool is_oct_digit(codepoint_t cp) {
			return ('0' <= cp && cp <= '7');
		}

		constexpr bool is_bin_digit(codepoint_t cp) {
			return cp == '1' || cp == '0';
		}

		constexpr bool is_quote(codepoint_t cp) {
			return cp == '"';
		}

		constexpr bool is_zero_digit(codepoint_t cp) {
			return cp == '0';
		}

		constexpr bool is_non_zero_digit(codepoint_t cp) {
			return !is_zero_digit(cp) && is_digit(cp);
		}

		constexpr bool is_hspace(codepoint_t cp) {
			return cp == ' ' || cp == '\t';
		}

		constexpr bool is_vspace(codepoint_t cp) {
			return cp == '\r' || cp == '\n' || cp == '\v';
		}

		constexpr bool is_space(codepoint_t cp) {
			return is_hspace(cp) || is_vspace(cp);
		}

      constexpr bool is_comment_head(codepoint_t cp) {
         return cp == '#';
      }

		constexpr bool is_ident_head(codepoint_t cp) {
			return (is_ascii(cp) && is_letter(cp)) || cp == '_';
		}

		constexpr bool is_ident_body(codepoint_t cp) {
			return is_ident_head(cp) || is_digit(cp);
		}

		constexpr bool iequal(codepoint_t lhs, codepoint_t rhs) {
			auto diff = lhs < rhs ? rhs - lhs : lhs - rhs;
			return lhs == rhs
				|| ((diff == 32)
				&& is_letter(lhs)
				&& is_letter(rhs));
		}
	} // ascii
} // rush

#endif // RUSH_CORE_ASCII_HPP
