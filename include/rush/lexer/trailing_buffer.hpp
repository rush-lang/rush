#pragma once

#ifndef RUSH_LEXER_TRAILING_BUFFER_HPP
#define RUSH_LEXER_TRAILING_BUFFER_HPP

#include "rush/core/ring.hpp"
#include "rush/core/algorithm.hpp"
#include "rush/core/iterator_ref.hpp"

#include <array>
#include <memory>

namespace rush {
	template <
		typename InIter,
		std::size_t N,
		typename Buffer = rush::ring<typename std::iterator_traits<InIter>::value_type, N>>
	class trailing_buffer {

		class trailing_buffer_iterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = typename std::iterator_traits<InIter>::value_type;
			using pointer = typename std::iterator_traits<InIter>::pointer;
			using reference = typename std::iterator_traits<InIter>::reference;
			using difference_type = typename std::iterator_traits<InIter>::difference_type;

			trailing_buffer_iterator(
				trailing_buffer& buf,
				typename Buffer::iterator iter,
				bool end)
					: _buf(buf)
					, _iter(iter)
					, _end(end) {}

			reference operator *() const { return _iter.operator*(); }
			pointer operator ->() const { return _iter.operator->(); }

			trailing_buffer_iterator& operator ++() {
				_end = !_buf.advance(_iter);
				return *this;
			}

			trailing_buffer_iterator operator ++(int) {
				auto temp = *this;
				_end = !_buf.advance(_iter);
				return temp;
			}

			friend bool operator == (
				trailing_buffer_iterator const& lhs,
				trailing_buffer_iterator const& rhs) {
					return lhs.iter_or_end() == rhs.iter_or_end()
						&& lhs._end == rhs._end;
				}

			friend bool operator != (
				trailing_buffer_iterator const& lhs,
				trailing_buffer_iterator const& rhs) {
					return !(lhs == rhs);
				}

		private:
			bool _end;
			trailing_buffer& _buf;
			typename Buffer::iterator _iter;

			typename Buffer::iterator iter_or_end() const {
				return _end ? _buf._buf.end() : _iter;
			}
		};

	public:
		using iterator_type = InIter;
		using buffer_type = Buffer;

		using value_type = typename buffer_type::value_type;
		using difference_type = typename buffer_type::difference_type;
		using size_type = typename buffer_type::size_type;

		using iterator = trailing_buffer_iterator;
		using const_iterator = trailing_buffer_iterator;

		trailing_buffer(InIter first, InIter last)
			: _first(first)
			, _last(last) { initialize(); }

		iterator begin() { return { *this, _buf.begin(), false }; }
		iterator end() { return { *this, _buf.end(), true }; }

	private:
		Buffer _buf;
		InIter _first, _last;

		void initialize() {
			rush::copy_n(
				rush::iterator_ref(_first), N,
				std::back_inserter(_buf));
		}

		bool advance(typename Buffer::iterator& it) {
			++it;
			if (it == _buf.end()) {
				if (_first != _last) {
					_buf.push_back(*_first);
					++_first;
				} else {
					return false;
				}
			}
			return true;
		}
	};

	template <std::size_t N, typename InIter>
	constexpr auto make_trailing_buffer(InIter first, InIter last)
		-> trailing_buffer<InIter, N> { return { first, last }; }
}

#endif // RUSH_LEXER_TRAILING_BUFFER_HPP
