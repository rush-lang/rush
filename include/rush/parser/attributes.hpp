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

#ifndef RUSH_PARSER_ATTRIBUTES_HPP
#define RUSH_PARSER_ATTRIBUTES_HPP

#include <cstdint>

namespace rush {

	// bits 1-3: symbol_kind
	// bits 4-7: access_modifier
	// bits 8-9: storage_class_specifier
	using symbol_attributes_t = std::uint32_t;

	// \brief An enumeration of mutually exclusive symbol types.
	enum class symbol_kind {
      constant,
      variable,
      function,
      struct_,
      class_,
      enum_,
      concept,
      interface,
      module,
   };

	// \brief An enumeration of mutually exclusive access modifiers.
	enum class access_modifier : std::uint8_t {
		internal,
		exported,
		public_,
		private_,
		protected_,
	};

	// \brief An enumeration of mutually exclusive storage class specifiers.
	enum class storage_class_specifier : std::uint8_t {
		auto_,
		static_,
		thread_local_
	};
} // rush

#endif // RUSH_PARSER_ATTRIBUTES_HPP
