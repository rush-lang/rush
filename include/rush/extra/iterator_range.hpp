#pragma once

#ifndef RUSH_CORE_ITERATOR_RANGE_HPP
#define RUSH_CORE_ITERATOR_RANGE_HPP

#include <utility>
#include <iterator>

namespace rush {
	namespace detail {
		template <typename ContainerT>
		auto adl_begin(ContainerT&& cont) {
			using std::begin;
			return begin(cont);
		}

		template <typename ContainerT>
		auto adl_end(ContainerT&& cont) {
			using std::end;
			return end(cont);
		}
	}

	template <typename IterT>
	class iterator_range {
	public:
		using iterator = IterT;
		using const_iterator = IterT;

		using value_type = typename std::iterator_traits<IterT>::value_type;
		using reference = typename std::iterator_traits<IterT>::reference;
		using pointer = typename std::iterator_traits<IterT>::pointer;
		using difference_type = typename std::iterator_traits<IterT>::difference_type;
		using iterator_category = typename std::iterator_traits<IterT>::iterator_category;

		template <typename ContainerT>
		iterator_range(ContainerT&& cont)
			: _first(detail::adl_begin(cont))
			, _last(detail::adl_end(cont)) {}

		iterator_range(IterT first, IterT last)
			: _first(first)
			, _last(last) {}

		template <typename ContainerT>
		iterator_range& operator = (ContainerT& cont) {
			_first = detail::adl_begin(cont);
			_last = detail::adl_end(cont);
			return *this;
		}

		iterator_range& operator = (iterator_range const& other) {
			_first = other.begin();
			_last = other.end();
			return *this;
		}


      bool empty() const noexcept { return _first == _last; }
		bool size() const noexcept { return 0; }

		const_iterator begin() const { return _first; }
		const_iterator end() const { return _last; }

		const_iterator cbegin() const { return _first; }
		const_iterator cend() const { return _last; }

	private:
		IterT _first, _last;
	};

	template <typename IterT>
	iterator_range<IterT> make_iterator_range(IterT first, IterT last) {
		return { std::move(first), std::move(last) };
	}

	template <typename IterT>
	iterator_range<IterT> make_iterator_range(std::pair<IterT, IterT> p) {
		return { std::move(p.first), std::move(p.second) };
	}
} // rush

#endif // RUSH_CORE_ITERATOR_RANGE_HPP
