#include "rush/ast/types/builtin.hpp"
#include <memory>
#include <array>

namespace rush::ast::types {

	std::unique_ptr<builtin_type> make_builtin_type(std::string name) {
		return std::make_unique<builtin_type>(std::move(name));
	}

   std::array<std::unique_ptr<builtin_type>, 16> const _builtin_types = {
      make_builtin_type("<error-type>"),
      make_builtin_type("<inferred-type>"),
      make_builtin_type("void"),
	   make_builtin_type("bool"),
	   make_builtin_type("byte"),
	   make_builtin_type("sbyte"),
	   make_builtin_type("short"),
	   make_builtin_type("ushort"),
	   make_builtin_type("int"),
	   make_builtin_type("uint"),
	   make_builtin_type("long"),
	   make_builtin_type("ulong"),
	   make_builtin_type("float"),
	   make_builtin_type("double"),
	   make_builtin_type("string"),
	   make_builtin_type("chars"),
   };

   // sentinals
	ast::type_ref const error_type = *_builtin_types[0];
	ast::type_ref const inferred_type = *_builtin_types[1];

	// known/built-in types
	ast::type_ref const void_type = *_builtin_types[2];
	ast::type_ref const bool_type = *_builtin_types[3];
	ast::type_ref const byte_type = *_builtin_types[4];
	ast::type_ref const sbyte_type = *_builtin_types[5];
	ast::type_ref const short_type = *_builtin_types[6];
	ast::type_ref const ushort_type = *_builtin_types[7];
	ast::type_ref const int_type = *_builtin_types[8];
	ast::type_ref const uint_type = *_builtin_types[9];
	ast::type_ref const long_type = *_builtin_types[10];
	ast::type_ref const ulong_type = *_builtin_types[11];
	ast::type_ref const float_type = *_builtin_types[12];
	ast::type_ref const double_type = *_builtin_types[13];
	ast::type_ref const string_type = *_builtin_types[14];
	ast::type_ref const char_type = *_builtin_types[15];
}
