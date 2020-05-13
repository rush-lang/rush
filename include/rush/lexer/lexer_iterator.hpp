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

#ifndef RUSH_LEXER_LEXER_ITERATOR_HPP
#define RUSH_LEXER_LEXER_ITERATOR_HPP

#include "rush/core/ascii.hpp"
#include "rush/core/location.hpp"

#include <iterator>

using namespace rush;
using namespace ascii;

namespace rush {
	template <typename InIter>
	class lexer_iterator {
	public:
		struct element {
		private:
			rush::location _loc;
			typename std::iterator_traits<InIter>::value_type _val;
			friend class lexer_iterator;

		public:
			element()
				: _val { }
				, _loc { } {}

			element(typename std::iterator_traits<InIter>::value_type val, rush::location loc)
				: _val(val)
				, _loc(loc) {}

			auto elem() const noexcept -> decltype(_val) { return _val; }
			auto location() const noexcept -> rush::location const& { return _loc; }
		};

		using iterator_category = std::forward_iterator_tag;
		using value_type = element;
		using pointer = value_type const*;
		using reference = value_type const&;
		using difference_type = typename std::iterator_traits<InIter>::difference_type;

		lexer_iterator(InIter it, location loc = {})
			: _iter { it }
			, _e { *it, loc } {}

		lexer_iterator(InIter it, typename std::iterator_traits<InIter>::value_type val, location loc = {})
			: _iter { it }
			, _e { val, loc } {}

		reference operator *() const {
			return _elem();
		}

		pointer operator ->() const {
			return std::addressof(_elem());
		}

		lexer_iterator& operator ++() {
			_increment();
			return *this;
		}

		lexer_iterator const operator ++(int) {
			auto temp = *this;
			_increment();
			return temp;
		}

		friend bool operator == (
			lexer_iterator const& lhs,
			lexer_iterator const& rhs) noexcept {
			return lhs._iter == rhs._iter;
		}

		friend bool operator != (
			lexer_iterator const& lhs,
			lexer_iterator const& rhs) noexcept {
			return !(lhs == rhs);
		}

	private:
		InIter _iter;
		element _e;

		reference _elem() const {
			return _e;
		}

		void _increment() {
			_e._loc = is_newline(*_iter)
				? _e._loc.next_line()
				: _e._loc.next_column();
			++_iter;
			_e._val = *_iter;
		}
	};
}

#endif // RUSH_LEXER_LEXER_ITERATOR_HPP
