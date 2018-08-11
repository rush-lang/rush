#include "rush/ast/type.hpp"
#include <unordered_map>

namespace rush::ast {
	type make_type(scope& sc, std::string name) {
		sc.insert(sema::make_type_symbol(name));
		return type { sc.lookup_local(name) };
	}

	// sentinals
	type const undefined_type = make_type(rush::global_scope, "<undefined>");
	type const error_type = make_type(rush::global_scope, "<error-type>");

	// known/built-in types
	type const void_type = make_type(rush::global_scope, "void");
	type const bool_type = make_type(rush::global_scope, "bool");
	type const byte_type = make_type(rush::global_scope, "byte");
	type const sbyte_type = make_type(rush::global_scope, "sbyte");
	type const short_type = make_type(rush::global_scope, "short");
	type const ushort_type = make_type(rush::global_scope, "ushort");
	type const int_type = make_type(rush::global_scope, "int");
	type const uint_type = make_type(rush::global_scope, "uint");
	type const long_type = make_type(rush::global_scope, "long");
	type const ulong_type = make_type(rush::global_scope, "ulong");
	type const float_type = make_type(rush::global_scope, "float");
	type const double_type = make_type(rush::global_scope, "double");
	type const string_type = make_type(rush::global_scope, "string");
	type const char_type = make_type(rush::global_scope, "chars");

	int type_precedence(type const& t) {
		return 0;
	}

	// \brief Returns the nearest base type of type.
	type base_of(type const& type) {
		// if (type.is_builtin())
		// 	return undefined_type;

		return undefined_type;
	}

	// \brief Returns the common type between two types,
	type intersection_of(type const& lhs, type const& rhs) {
		// if (lhs.is_builtin() && rhs.is_builtin()) {
		// 	return type_precedence(lhs) < type_precedence(rhs)
		// 		? rhs : lhs;
		// }

		return undefined_type;
	}
} // rush