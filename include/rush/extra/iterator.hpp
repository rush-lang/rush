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

	template <typename InIter1, typename InIter2>
	std::enable_if_t<std::is_same_v<
		std::decay_t<InIter1>,
		std::decay_t<InIter2>>,
	InIter1> advance__new(InIter1 it, InIter2 last) {
			while (it != last) { ++it; }
			return it;
		}

	template <typename InIter1, typename InIter2>
	std::enable_if_t<std::is_same_v<
		std::decay_t<InIter1>,
		std::decay_t<InIter2>>,
	InIter1> advance__new(
		InIter1 it,
		InIter2 last,
		typename std::iterator_traits<InIter1>::difference_type n) {
			while (it != last && n > 0) { ++it; --n; }
			return it;
		}

	template <typename InIter1, typename InIter2, typename Pred>
	std::enable_if_t<std::is_same_v<
		std::decay_t<InIter1>,
		std::decay_t<InIter2>>,
	InIter1> advance_if__new(
		InIter1 it,
		InIter2 last,
		Pred predicate) {
			while (it != last && predicate(*it)) ++it;
			return it;
		}

	template <typename InIter1, typename InIter2, typename Pred>
	std::enable_if_t<std::is_same_v<
		std::decay_t<InIter1>,
		std::decay_t<InIter2>>,
	InIter1> advance_n_if__new(
		InIter1 it,
		InIter2 last,
		typename std::iterator_traits<InIter1>::difference_type n,
		Pred predicate) {
			while (it != last && predicate(*it)) ++it;
			return it;
		}
} // rush

#endif // RUSH_CORE_ITERATOR_HPP
