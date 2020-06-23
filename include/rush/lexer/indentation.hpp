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

#ifndef RUSH_LEXER_INDENTATION_HPP
#define RUSH_LEXER_INDENTATION_HPP

#include "rush/core/ascii.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace rush {

	enum class indentation_style : std::uint8_t {
		unknown,
		spaces,
		tabs
	};

	// \brief Simple class that tracks indentation depth and style.
	// handles measuring, increases, and decreases of indentation.
	class indentation final {
		constexpr indentation(std::size_t depth, std::size_t inlen, indentation_style style) noexcept
			: _depth { depth }
			, _inlen { inlen }
			, _style { style } {}

	public:
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;

		constexpr indentation() noexcept
			: _depth { 0 }
			, _inlen { 0 }
			, _style { indentation_style::unknown } {}

		size_type depth() const noexcept {
			return _depth;
		}

		indentation_style style() const noexcept {
			return _style;
		}

		indentation& increment() noexcept {
			++_depth;
			return *this;
		}

		indentation& decrement() noexcept {
			--_depth;
			return *this;
		}

      static difference_type difference(indentation const& lhs, indentation const& rhs) {
         return static_cast<difference_type>(lhs.depth())
              - static_cast<difference_type>(rhs.depth());
      }

		template <typename InIter>
		indentation measure(InIter first, InIter last) {
			auto pin = first;

			if (advance_if(first, last, [this](auto cp) {
				return this->is_indent_char(cp);
			})) {
				auto d = distance_to_depth(pin, first);
				return { d, _inlen, _style };
			}

			return { 0, _inlen, _style };
		}

		static inline indentation spaces(std::size_t depth) noexcept {
			return { 1, depth, indentation_style::spaces };
		}

		static inline indentation tabs(std::size_t depth) noexcept {
			return { depth, 1, indentation_style::tabs };
		}

	private:
		std::size_t _depth;
		std::size_t _inlen; // number of indent chars needed to be considered an indentation.
		indentation_style _style;

		bool is_indent_char(codepoint_t cp) {
			switch (_style) {
				case indentation_style::tabs: return cp == '\t';
				case indentation_style::spaces: return cp == ' ';
				default:
					if (cp == '\t') { _style = indentation_style::tabs; return true; }
					if (cp == ' ') { _style = indentation_style::spaces; return true; }
					return false;
			}
		}

		template <typename InIter>
		std::size_t distance_to_depth(InIter first, InIter last) {
			assert(_style != indentation_style::unknown && "expected a known indentation style.");
			auto dist = std::distance(first, last);
			if (_inlen == 0) { _inlen = dist; }

			switch (_style) {
				case indentation_style::spaces: return dist / _inlen;
				case indentation_style::tabs: return dist / _inlen;
				default: throw;
			}
		}
	};

	bool operator == (indentation const& lhs, indentation const& rhs) {
		// todo: throw/assert the styles are the same (maybe? haven't decided how to handle this yet.)
		return lhs.style() == rhs.style() && lhs.depth() == rhs.depth();
	}

	bool operator != (indentation const& lhs, indentation const& rhs) {
		return !(lhs == rhs);
	}


	bool operator < (indentation const& lhs, indentation const& rhs) {
		return lhs.depth() < rhs.depth();
	}

	bool operator > (indentation const& lhs, indentation const& rhs) {
		return rhs < lhs;
	}

	bool operator <= (indentation const& lhs, indentation const& rhs) {
		return !(rhs < lhs);
	}

	bool operator >= (indentation const& lhs, indentation const& rhs) {
		return !(lhs < rhs);
	}

} // rush






#endif // RUSH_LEXER_INDENTATION_HPP
