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

#ifndef RUSH_CORE_RESET_HPP
#define RUSH_CORE_RESET_HPP

#include "rush/extra/function_ref.hpp"

#include <functional>

namespace rush {
	template <typename T>
	class value_reset final {
	public:
		value_reset(T& ref, T&& set)
			: value_reset(ref, [&set]() { return std::forward<T>(set); }) {}

		value_reset(T& ref, rush::function_ref<T()> set)
			: value_reset(ref, ref, std::move(set), [this]() { return this->_tmp; }) {}

		value_reset(T& ref, rush::function_ref<T()> set, std::function<T()> get)
			: value_reset(ref, ref, std::move(set), std::move(get)) {}

      ~value_reset() { _ref = _get(); }

	private:
		value_reset(T& ref, T tmp, rush::function_ref<T()> set, std::function<T()> get)
			: _get { std::move(get) }
			, _tmp { std::move(tmp) }
			, _ref { ref }
			{ _ref = set(); }

		std::function<T()> _get;
		T _tmp;
		T& _ref;
	};

   template <typename T>
   rush::value_reset<T> guard(T& ref, T&& set) {
      return { ref, std::forward<T>(set) };
   }

   template <typename T>
   rush::value_reset<T> guard(T& ref, rush::function_ref<T()> set) {
      return { ref, std::move(set) };
   }

   template <typename T>
   rush::value_reset<T> guard(T& ref, rush::function_ref<T()> set, std::function<T()> get) {
      return { ref, std::move(set), std::move(get) };
   }
}


#endif // RUSH_CORE_RESET_HPP
