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
} // rush

#endif // RUSH_CORE_ITERATOR_HPP
