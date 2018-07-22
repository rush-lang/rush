#pragma once

#ifndef RUSH_CORE_ITERATOR_HPP
#define RUSH_CORE_ITERATOR_HPP


namespace rush {
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
