#include "rush/ast/types/builtin.hpp"
#include <memory>

namespace rush::ast::types {

	std::unique_ptr<builtin_type> make_builtin_type(std::string name) {
		return std::make_unique<builtin_type>(std::move(name));
	}

   // sentinals
	std::unique_ptr<builtin_type> const error_type = make_builtin_type("<error-type>");
	std::unique_ptr<builtin_type> const inferred_type = make_builtin_type("<inferred-type>");

	// known/built-in types
	std::unique_ptr<builtin_type> const void_type = make_builtin_type("void");
	std::unique_ptr<builtin_type> const bool_type = make_builtin_type("bool");
	std::unique_ptr<builtin_type> const byte_type = make_builtin_type("byte");
	std::unique_ptr<builtin_type> const sbyte_type = make_builtin_type("sbyte");
	std::unique_ptr<builtin_type> const short_type = make_builtin_type("short");
	std::unique_ptr<builtin_type> const ushort_type = make_builtin_type("ushort");
	std::unique_ptr<builtin_type> const int_type = make_builtin_type("int");
	std::unique_ptr<builtin_type> const uint_type = make_builtin_type("uint");
	std::unique_ptr<builtin_type> const long_type = make_builtin_type("long");
	std::unique_ptr<builtin_type> const ulong_type = make_builtin_type("ulong");
	std::unique_ptr<builtin_type> const float_type = make_builtin_type("float");
	std::unique_ptr<builtin_type> const double_type = make_builtin_type("double");
	std::unique_ptr<builtin_type> const string_type = make_builtin_type("string");
	std::unique_ptr<builtin_type> const char_type = make_builtin_type("chars");
}
