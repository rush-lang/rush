#include "rush/ast/type.hpp"

namespace rush::ast {
	// known/built-in types
	type const none_type = {}; // sentinal
	type const void_type = {};
	type const bool_type = {};
	type const byte_type = {};
	type const sbyte_type = {};
	type const short_type = {};
	type const ushort_type = {};
	type const int_type = {};
	type const uint_type = {};
	type const long_type = {};
	type const ulong_type = {};
	type const float_type = {};
	type const double_type = {};
	type const string_type = {};
	type const char_type = {};

	// \brief Returns the nearest base type of type.
	type base_of(type const& type) {
		return none_type;
	}

	// \brief Returns the common type between two types,
	type intersection_of(type const& lhs, type const& rhs) {
		return none_type;
	}
} // rush