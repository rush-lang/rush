#include "rush/ast/types/builtin.hpp"

namespace rush::ast {

	type make_primitive_type(std::string const& name) {
		return type { name };
	}

	// sentinals
	type const error_type = make_primitive_type("<error-type>");

	// known/built-in types
	type const void_type = make_primitive_type("void");
	type const bool_type = make_primitive_type("bool");
	type const byte_type = make_primitive_type("byte");
	type const sbyte_type = make_primitive_type("sbyte");
	type const short_type = make_primitive_type("short");
	type const ushort_type = make_primitive_type("ushort");
	type const int_type = make_primitive_type("int");
	type const uint_type = make_primitive_type("uint");
	type const long_type = make_primitive_type("long");
	type const ulong_type = make_primitive_type("ulong");
	type const float_type = make_primitive_type("float");
	type const double_type = make_primitive_type("double");
	type const string_type = make_primitive_type("string");
	type const char_type = make_primitive_type("chars");
}