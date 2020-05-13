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

#ifndef RUSH_LEXER_LOOKAHEAD_HPP
#define RUSH_LEXER_LOOKAHEAD_HPP

#include "rush/extra/ring.hpp"
#include "rush/extra/iterator.hpp"
#include "rush/extra/algorithm.hpp"
#include "rush/extra/iterator_ref.hpp"

#include <iosfwd>
#include <iterator>

namespace rush {
	template <typename InIter, std::size_t N>
	class lookahead_buffer {
	public:
		using buffer_type = rush::ring<
			typename std::iterator_traits<InIter>::value_type, N>;

		using value_type = typename buffer_type::value_type;
		using reference = typename buffer_type::reference;
		using pointer = typename buffer_type::pointer;
		using size_type = typename buffer_type::size_type;
		using difference_type = typename buffer_type::difference_type;

		lookahead_buffer(InIter first, InIter last)
			: _iter(first)
			, _last(last) { _initialize(); }

		bool empty() const noexcept {
			return _finished()
				&& _buf.empty();
		}

		size_type size() const noexcept {
			return _buf.size();
		}

		size_type capacity() const noexcept {
			return _buf.capacity();
		}

		reference peek(difference_type offset = 0) {
			_check_offset(offset);
			return *rush::advance__new(
				_buf.begin(),
				_buf.end(),
				offset);
		}

		void skip() {
			if (!_finished()) {
				_buf.push_back(*_iter);
				++_iter;
			} else {
				_check_offset(0);
				_buf.pop_front();
			}
		}

		void skip(difference_type offset) {
			for (; offset > 0; --offset) skip();
		}

		value_type next() {
			auto prev = std::move(_buf.front()); skip();
			return std::move(prev);
		}

	private:
		InIter _iter, _last;
		buffer_type _buf;

		bool _finished() const noexcept {
			return _iter == _last;
		}

		void _check_offset(difference_type offset) const {
			if (offset >= _buf.size()) throw std::out_of_range("offset out of range");
		}

		void _initialize() {
			rush::copy_n(
				rush::iterator_ref(_iter), _last, N,
				std::back_inserter(_buf));
		}
	};

	template <std::size_t N, typename Container>
	lookahead_buffer<typename Container::iterator, N> lookahead(Container& cont) {
		return { begin(cont), end(cont) };
	}

	template <std::size_t N, typename InIter>
	lookahead_buffer<InIter, N> lookahead(InIter first, InIter last) {
		return { first, last };
	}
}

#endif // RUSH_LEXER_LOOKAHEAD_HPP
