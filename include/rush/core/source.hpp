#pragma once

#ifndef RUSH_CORE_SOURCE_HPP
#define RUSH_CORE_SOURCE_HPP

#include "rush/core/path.hpp"
#include "rush/core/location.hpp"

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <iterator>

namespace rush {

	namespace detail {
		template <typename Iter>
		class source_iterator : public std::iterator<
			std::input_iterator_tag,
			typename std::iterator_traits<Iter>::value_type,
			typename std::iterator_traits<Iter>::difference_type,
			typename std::iterator_traits<Iter>::pointer,
			typename std::iterator_traits<Iter>::reference> {
		public:

			// \brief Returns the current line the iterator points to within source.
			source_index_t line() const noexcept {
				return _line;
			}

			// \brief Returns the current column the iterator points to within source.
			source_index_t column() const noexcept {
				return _column;
			}

			// reference operator *() noexcept {
			// 	return *_iter;
			// }

		private:
			source_index_t _line;
			source_index_t _column;
			Iter _iter;
		};
	}

	class source {
	public:
		using iterator = detail::source_iterator<std::string::iterator>;
		using const_iterator = detail::source_iterator<std::string::const_iterator>;

		std::uint32_t id() const noexcept;

		filesystem::path filename() const noexcept;
		filesystem::path directory() const noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		source(std::istream&);
		source(filesystem::path);

	private:
		std::istream& _content;
	};
} // rush

#endif // RUSH_CORE_SOURCE_HPP
