#pragma once

#ifndef RUSH_CORE_ITERATOR_HPP
#define RUSH_CORE_ITERATOR_HPP


namespace rush {
	template <typename InIter, typename Pred>
	auto advance_if(InIter& first, InIter const& last, Pred predicate) {
		auto start = first;
		while (first != last && predicate(*first)) ++first;
		return start != first;
	}

	template <typename InIter, typename Pred>
	auto next_if(InIter const& first, InIter const& last, Pred predicate) {
		auto temp = first;
		advance_if(temp, last, predicate);
		return temp;
	}
} // rush

#endif // RUSH_CORE_ITERATOR_HPP
