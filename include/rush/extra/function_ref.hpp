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

#ifndef RUSH_CORE_EXTRAS_HPP
#define RUSH_CORE_EXTRAS_HPP

#include <type_traits>

namespace rush {


	/// ORIGINAL SOURCE: The LLVM project.
	/// An efficient, type-erasing, non-owning reference to a callable. This
	/// is intended for use as the type of a function parameter that is not used
	/// after the function in question returns.
	///
	/// This class does not own the callable, so it is not in general save to store
	/// a function_ref
	template <typename Fn>
	class function_ref;

	template <typename Ret, typename... Params>
	class function_ref<Ret(Params...)> {
		Ret (*callback)(intptr_t callable, Params... params) = nullptr;
		intptr_t callable;

		template <typename Callable>
		static Ret callback_fn(intptr_t callable, Params... params) {
			return (*reinterpret_cast<Callable*>(callable))(
				std::forward<Params>(params)...);
		}
	public:
		function_ref() = default;
		function_ref(std::nullptr_t) {}

		template <typename Callable>
		function_ref(Callable&& callable,
			std::enable_if_t<
				!std::is_same_v<std::remove_reference_t<Callable>,
					function_ref>>* = nullptr)
			: callback(callback_fn<std::remove_reference_t<Callable>>)
			, callable(reinterpret_cast<intptr_t>(&callable)) {}

		Ret operator ()(Params... args) const {
			return callback(callable, std::forward<Params>(args)...);
		}

		operator bool() const { return callback; }
	};
} // rush

#endif // RUSH_CORE_EXTRAS_HPP
