#include "rush/ast/types/builtin.hpp"
#include <memory>
#include <array>

namespace rush::ast::types {

   std::unique_ptr<builtin_integral_type> make_builtin_integral_type(ast::integral_kind kind, bool sign) {
		return std::make_unique<builtin_integral_type>(kind, sign);
	}

   std::unique_ptr<builtin_floating_type> make_builtin_floating_type(ast::floating_kind k) {
		return std::make_unique<builtin_floating_type>(k);
	}

   std::array<std::unique_ptr<builtin_type>, 16> const _builtin_types = {
      std::make_unique<builtin_void_type>(),
      std::make_unique<builtin_void_type>(),
      std::make_unique<builtin_void_type>(),
      std::make_unique<builtin_bool_type>(),
	   make_builtin_integral_type(ast::integral_kind::byte, false),
	   make_builtin_integral_type(ast::integral_kind::byte, true),
	   make_builtin_integral_type(ast::integral_kind::word, true),
	   make_builtin_integral_type(ast::integral_kind::word, false),
	   make_builtin_integral_type(ast::integral_kind::dword, true),
	   make_builtin_integral_type(ast::integral_kind::dword, false),
	   make_builtin_integral_type(ast::integral_kind::qword, true),
	   make_builtin_integral_type(ast::integral_kind::qword, false),
	   make_builtin_floating_type(ast::floating_kind::ieee32),
	   make_builtin_floating_type(ast::floating_kind::ieee64),
	   std::make_unique<builtin_void_type>(),
      std::make_unique<builtin_void_type>(),
   };

   // sentinals
	ast::type_ref const undefined = *_builtin_types[0];
	ast::type_ref const error_type = *_builtin_types[1];

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
