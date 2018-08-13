#pragma once

#ifndef RUSH_SEMA_ATTRIBUTES_HPP
#define RUSH_SEMA_ATTRIBUTES_HPP

#include <cstdint>

namespace rush::sema {

	// bits 1-3: symbol_kind
	// bits 4-7: access_modifier
	// bits 8-9: storage_class_specifier
	using symbol_attributes_t = std::uint32_t;

	// \brief An enumeration of mutually exclusive symbol types.
	enum class symbol_kind : std::uint8_t {
		undefined,
		variable,
		constant,
		function,
		type,
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
} // rush::sema

#endif // RUSH_SEMA_ATTRIBUTES_HPP
