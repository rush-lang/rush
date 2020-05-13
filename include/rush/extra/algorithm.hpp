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

#ifndef RUSH_CORE_ALGORITHM_HPP
#define RUSH_CORE_ALGORITHM_HPP

#include <algorithm>

namespace rush {
	template <typename InIter1, typename InIter2, typename OutIter>
	inline std::enable_if_t<
		std::is_same_v<
			std::decay_t<InIter1>,
			std::decay_t<InIter2>>,
		OutIter> copy(InIter1 first, InIter2 last, OutIter out)  {
			// rush::copy* guarantees the first parameter is used
			// to iterate over the range; useful when passing rush::iterator_reference_wrapper.
			// std::copy* does not hold this same guarantee (though it is the common case)
			// and requires both first and last to be of the exact same type ; may fail here.
			for (; first != last; ++first, (void) ++out)
				*out = *first;
			return out;
		}

	template <typename InIter, typename OutIter, typename SizeT>
	OutIter copy_n(InIter first, SizeT n, OutIter out) {
		for (; n > 0; --n, ++first, (void) ++out)
			*out = *first;
		return out;
	}

	// A version of copy_n that will copy n elements where n is either
	// the distance between first and last or the supplied count parameter
	// when said distance is greater than count.
	template <typename InIter1, typename InIter2, typename OutIter, typename SizeT>
	inline std::enable_if_t<
		std::is_same_v<
			std::decay_t<InIter1>,
			std::decay_t<InIter2>>,
		OutIter> copy_n(InIter1 first, InIter2 last, SizeT count, OutIter out) {
			for (; first != last && count > 0;
				++first, --count, ++out) { *out = *first; }
			return out;
		}

	template <typename InIter1, typename InIter2, typename OutIter, typename Pred>
	inline std::enable_if_t<
		std::is_same_v<
			std::decay_t<InIter1>,
			std::decay_t<InIter2>>,
		OutIter> copy_while(InIter1 first, InIter2 last, Pred predicate, OutIter out) {
			for (; first != last && predicate(*first); ++first, ++out)
				*out = *first;
			return out;
		}

} // rush

#endif // RUSH_CORE_ALGORITHM_HPP
