#pragma once

#ifndef RUSH_CORE_ITERATOR_HPP
#define RUSH_CORE_ITERATOR_HPP

#include <iterator>

namespace rush {

	template <typename InIter>
	auto advance(
		InIter& it,
		InIter const& last,
		typename std::iterator_traits<InIter>::difference_type n = -1) -> bool {
			auto first = it;
			while (it != last && n > 0) { ++it; --n; }
			return it != first;
		}

	template <typename InIter, typename Pred>
	auto advance_if(
		InIter& it,
		InIter const& last,
		Pred predicate) -> decltype(predicate(*it), bool()) {
			auto first = it;
			while (it != last && predicate(*it)) ++it;
			return it != first;
		}
} // rush

#endif // RUSH_CORE_ITERATOR_HPP
