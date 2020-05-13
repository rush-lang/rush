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

#ifndef RUSH_CORE_ITERATOR_REF_HPP
#define RUSH_CORE_ITERATOR_REF_HPP

#include <iterator>

namespace rush {
	template <typename IterT>
	class iterator_reference_wrapper {
	public:
		using iterator_category = typename std::iterator_traits<IterT>::iterator_category;
		using value_type = typename std::iterator_traits<IterT>::value_type;
		using pointer = typename std::iterator_traits<IterT>::pointer;
		using reference = typename std::iterator_traits<IterT>::reference;
		using difference_type = typename std::iterator_traits<IterT>::difference_type;

		iterator_reference_wrapper(IterT& iter)
			: _iter(iter) {}

		iterator_reference_wrapper(iterator_reference_wrapper&) = default;
		iterator_reference_wrapper& operator = (iterator_reference_wrapper const&) = default;

		iterator_reference_wrapper(iterator_reference_wrapper&&) = default;
		iterator_reference_wrapper& operator = (iterator_reference_wrapper&&) = default;

		reference operator [](difference_type index) { return _iter[index]; }
		reference operator *() const { return *_iter; }
		pointer operator ->() const { return _iter; }

		IterT operator ++(int) noexcept { return _iter++; }
		iterator_reference_wrapper& operator ++() noexcept { ++_iter; return *this; }

		IterT operator --(int) noexcept { return _iter--; }
		iterator_reference_wrapper& operator --() noexcept { --_iter; return *this; }

		friend bool operator == (iterator_reference_wrapper const& lhs, iterator_reference_wrapper const& rhs) { return lhs._iter == rhs._iter; }
		friend bool operator == (iterator_reference_wrapper const& lhs, IterT const& rhs) { return lhs._iter == rhs; }
		friend bool operator == (IterT const& lhs, iterator_reference_wrapper const& rhs) { return lhs == rhs._iter; }
		friend bool operator != (iterator_reference_wrapper const& lhs, iterator_reference_wrapper const& rhs) { return !(lhs == rhs); }
		friend bool operator != (iterator_reference_wrapper const& lhs, IterT const& rhs) { return !(lhs == rhs); }
		friend bool operator != (IterT const& lhs, iterator_reference_wrapper const& rhs) { return !(lhs == rhs); }

		friend bool operator < (iterator_reference_wrapper const& lhs, iterator_reference_wrapper const& rhs) { return lhs._iter < rhs._iter; }
		friend bool operator > (iterator_reference_wrapper const& lhs, iterator_reference_wrapper const& rhs) { return rhs < lhs; }
		friend bool operator <= (iterator_reference_wrapper const& lhs, iterator_reference_wrapper const& rhs) { return !(rhs < lhs); }
		friend bool operator >= (iterator_reference_wrapper const& lhs, iterator_reference_wrapper const& rhs) { return !(lhs < rhs); }

	private:
		IterT& _iter;
	};

	template <typename IterT>
	iterator_reference_wrapper<IterT> iterator_ref(IterT& iter) {
		return iterator_reference_wrapper { iter };
	}
} // rush

namespace std {
	template <typename IterT>
	struct remove_reference<rush::iterator_reference_wrapper<IterT>>
		: public std::remove_reference<IterT> {};
}

#endif // RUSH_CORE_ITERATOR_REF_HPP
