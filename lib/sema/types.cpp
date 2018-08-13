#include "rush/sema/types.hpp"
#include "rush/sema/scope.hpp"

namespace rush::sema {

	auto const int_type_symbol = lookup("int");
	auto const long_type_symbol = lookup("long");
	auto const short_type_symbol = lookup("short");
	auto const uint_type_symbol = lookup("uint");
	auto const ulong_type_symbol = lookup("ulong");
	auto const ushort_type_symbol = lookup("ushort");
	auto const float_type_symbol = lookup("float");
	auto const double_type_symbol = lookup("double");

	static bool is_signed_integral_type(symbol const& s) {
		return int_type_symbol.id() == s.id()
			|| long_type_symbol.id() == s.id()
			|| short_type_symbol.id() == s.id();
	}

	static bool is_unsigned_integral_type(symbol const& s) {
		return uint_type_symbol.id() == s.id()
			|| ulong_type_symbol.id() == s.id()
			|| ushort_type_symbol.id() == s.id();
	}

	static bool is_floating_point_type(symbol const& s) {
		return lookup("float").id() == s.id()
			|| lookup("double").id() == s.id();
	}

	bool is_type_match(symbol const& lhs, symbol const& rhs) {
		assert(lhs.is_type() && "left-hand operand is not a type symbol.");
		assert(rhs.is_type() && "right-hand operand is not a type symbol.");
		return lhs.id() == rhs.id();
	}

	bool is_type_mismatch(symbol const& lhs, symbol const& rhs) {
		return !is_type_match(lhs, rhs);
	}

	std::tuple<symbol, bool> reduce_type_symbols(symbol const& lhs, symbol const& rhs) {
		if (is_type_match(lhs, rhs))
			return std::make_tuple(lhs, true);

		// if (is_signed_integral_type(lhs)) {
		// }

		return std::make_tuple(lookup("<error-type>"), false);
	}
}