#include "catch2/catch.hpp"
#include "rush/lexer/lexer.hpp"

#include <string_view>


template <typename Iter>
bool check_iterators(Iter first, Iter last, Iter out, std::size_t offset = -1) {
	if (offset != -1) {
		std::advance(first, offset);
		return out == first;
	}

	return out == last;
}


bool valid_integer_literal(std::string_view input, std::size_t offset = -1) {
	auto first = std::begin(input);
	auto last = std::end(input);
	auto out = rush::scan_integer_literal(first, last);
	return check_iterators(first, last, out, offset);
}

bool valid_floating_literal(std::string_view input, std::size_t offset = -1) {
	auto first = std::begin(input);
	auto last = std::end(input);
	auto out = rush::scan_floating_literal(first, last);
	return check_iterators(first, last, out, offset);
}

bool valid_string_literal(std::string_view input, std::size_t offset = -1) {
	auto first = std::begin(input);
	auto last = std::end(input);
	auto out = rush::scan_string_literal(first, last);
	return check_iterators(first, last, out, offset);
}

bool valid_identifier(std::string_view input, std::size_t offset = -1) {
	auto first = std::begin(input);
	auto last = std::end(input);
	auto out = rush::scan_identifier(first, last);
	return check_iterators(first, last, out, offset);
}



TEST_CASE( "rush::scan_integer_literal", "[unit][lexer]") {
	CHECK( valid_integer_literal("0") );
	CHECK( valid_integer_literal("9") );
	CHECK( valid_integer_literal("10") );
	CHECK( valid_integer_literal("123456789") );
	CHECK( valid_integer_literal("987654321") );

	// any leading-zero or non-digit should stop the scan,
	// and proceed to scan the next character as a seperate token.
	CHECK( valid_integer_literal("01", 1) );
	CHECK( valid_integer_literal("09", 1) );
	CHECK( valid_integer_literal("0123456789", 1) );
	CHECK( valid_integer_literal("0987654321", 1) );
	CHECK( valid_integer_literal("0_", 1) );
	CHECK( valid_integer_literal("1a", 1) );
	CHECK( valid_integer_literal("123_", 3) );
	CHECK( valid_integer_literal("123a", 3) );
	CHECK( valid_integer_literal("1.0", 1) );
	CHECK( valid_integer_literal("123.0", 3) );

	// FIXME: The following cause assert/terminate to be triggered. Currently no way to test this in Catch2
	// CHECK_FALSE( valid_integer_literal("_0") );
	// CHECK_FALSE( valid_integer_literal("a1") );
	// CHECK_FALSE( valid_integer_literal(".1") );
}

TEST_CASE( "rush::floating_literal", "[unit][lexer]" ) {
	CHECK( valid_floating_literal("1.0") );
	CHECK( valid_floating_literal("9.0") );
	CHECK( valid_floating_literal(".0") );
	CHECK( valid_floating_literal(".013") );

	CHECK_FALSE( valid_floating_literal("0") );
	CHECK_FALSE( valid_floating_literal("1") );
	CHECK_FALSE( valid_floating_literal("9") );
	CHECK_FALSE( valid_floating_literal("123") );
}

TEST_CASE( "rush::scan_string_literal", "[unit][lexer]" ) {
	CHECK( valid_string_literal("\"\"") ); 		// ""
	CHECK( valid_string_literal("\"a\"") ); 		// "a"
	CHECK( valid_string_literal("\"abc\"") ); 	// "abc"
	CHECK( valid_string_literal("\"0\"") );		// "0"
	CHECK( valid_string_literal("\"123\"") );		// "123"
	CHECK( valid_string_literal("\"\\\"\"\"", 4) ); // "\""

	CHECK( valid_string_literal("\"!\\\"£$%^&*(){}[]-=_+:;@~/?.,<>|\\\\\"") );
}

TEST_CASE( "rush::scan_identifier", "[unit][lexer]" ) {
	CHECK( valid_identifier("_") );
	CHECK( valid_identifier("_a") );
	CHECK( valid_identifier("_z") );
	CHECK( valid_identifier("_0") );
	CHECK( valid_identifier("_9") );
	CHECK( valid_identifier("_a0") );
	CHECK( valid_identifier("_z0") );
	CHECK( valid_identifier("_a9") );
	CHECK( valid_identifier("_z9") );

	CHECK( valid_identifier("a") );
	CHECK( valid_identifier("z") );
	CHECK( valid_identifier("A") );
	CHECK( valid_identifier("Z") );

	CHECK( valid_identifier("a") );
	CHECK( valid_identifier("b1") );
	CHECK( valid_identifier("c_") );
	CHECK( valid_identifier("d2_") );
	CHECK( valid_identifier("e_3") );
	CHECK( valid_identifier("f4_ab12__") );

	// like-joined
	CHECK( valid_identifier("abc123___") );
	CHECK( valid_identifier("abc___123") );
	CHECK( valid_identifier("___abc123") );
	CHECK( valid_identifier("___123abc") );

	// like-interspersed
	CHECK( valid_identifier("a_1b_2c_3") );
	CHECK( valid_identifier("_a1_b2_c3") );
	CHECK( valid_identifier("_1a_2b_3c") );

	// FIXME: The following cause assert/terminate to be triggered. Currently no way to test this in Catch2
	// REQUIRE_FALSE( valid_identifier("0") );
	// REQUIRE_FALSE( valid_identifier("9") );
	// REQUIRE_FALSE( valid_identifier("0a") );
	// REQUIRE_FALSE( valid_identifier("9z") );
	// REQUIRE_FALSE( valid_identifier("0a_") );
	// REQUIRE_FALSE( valid_identifier("9z_") );
}