#pragma once

#ifndef RUSH_CORE_RESET_HPP
#define RUSH_CORE_RESET_HPP

#include "rush/core/extras.hpp"

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
