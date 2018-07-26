#include "catch2/catch.hpp"
#include "rush/lexer/lexer.hpp"

#include <string_view>



template <typename Iter>
bool check_iterators(Iter first, Iter last, Iter out, std::size_t offset = -1) {
	if (offset == -1) return out == last;
	std::advance(first, offset);
	return out == first;
}

bool skipped_hspace(std::string_view input, std::size_t offset = -1) {
	auto first = begin(input);
	auto last = end(input);
	rush::skip_hspace(first, last);
	return check_iterators(begin(input), last, first, offset);
}

bool skipped_vspace(std::string_view input, std::size_t offset = -1) {
	auto first = begin(input);
	auto last = end(input);
	rush::skip_vspace(first, last);
	return check_iterators(begin(input), last, first, offset);
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

bool valid_lex(std::string_view input, std::initializer_list<rush::lexical_token> tokens) {
	return false;
}

TEST_CASE( "rush::lex", "[unit][lexer]" ) {

	SECTION( "keywords" ) {
		CHECK( valid_lex("if else while", {
			tok::if_keyword(),
			tok::else_keyword(),
			tok::while_keyword()
		}));

		CHECK_FALSE( valid_lex("_let var_ const1", {
			tok::let_keyword(),
			tok::var_keyword(),
			tok::const_keyword()
		}));
	}

	SECTION( "identifiers" ) {

		CHECK( valid_lex("a z A Z abc XYZ", {
			tok::identifier("a"),
			tok::identifier("z"),
			tok::identifier("A"),
			tok::identifier("Z"),
			tok::identifier("abc"),
			tok::identifier("XYZ"),
		}));

		CHECK( valid_lex("_ _a _z _0 _9 _a0 _z0 _a9 _z9", {
			tok::identifier("_"),
			tok::identifier("_a"),
			tok::identifier("_z"),
			tok::identifier("_0"),
			tok::identifier("_9"),
			tok::identifier("_a0"),
			tok::identifier("_z0"),
			tok::identifier("_a9"),
			tok::identifier("_z9"),
		}));

		CHECK( valid_lex("a1 c_ d2_ e_3 f4_ab12__", {
			tok::identifier("a1"),
			tok::identifier("c_"),
			tok::identifier("d2_"),
			tok::identifier("e_3"),
			tok::identifier("f4_ab12__"),
		}));

		// like-joined
		CHECK( valid_lex("abc123__ abc__123 __abc123 __123abc", {
			tok::identifier("abc123___"),
			tok::identifier("abc___123"),
			tok::identifier("___abc123"),
			tok::identifier("___123abc"),
		}));

		// like-interspersed
		CHECK( valid_lex("abc123__ abc__123 __abc123 __123abc", {
			tok::identifier("a_1b_2c_3"),
			tok::identifier("_a1_b2_c3"),
			tok::identifier("_1a_2b_3c"),
		}));
	}
}


TEST_CASE( "rush::skip_hspace", "[unit][lexer]" ) {

	SECTION( "leading space characters should be skipped." ) {
		CHECK( skipped_hspace(" . \t", 1) );
		CHECK( skipped_hspace(" ( \t", 1) );
		CHECK( skipped_hspace(" abc\t ", 1) );
		CHECK( skipped_hspace(" 123\t ", 1) );
		CHECK( skipped_hspace(" \r \t", 1) );
		CHECK( skipped_hspace(" \n \t", 1) );

		CHECK( skipped_hspace("   . \t", 3) );
		CHECK( skipped_hspace("   ( \t", 3) );
		CHECK( skipped_hspace("   abc\t ", 3) );
		CHECK( skipped_hspace("   123\t ", 3) );
		CHECK( skipped_hspace("   \r \t", 3) );
		CHECK( skipped_hspace("   \n \t", 3) );

		CHECK_FALSE( skipped_hspace(". \t", 1) );
		CHECK_FALSE( skipped_hspace("() \t", 1) );
		CHECK_FALSE( skipped_hspace("abc\t ", 1) );
		CHECK_FALSE( skipped_hspace("123\t ", 1) );
	}

	SECTION( "leading tab characters should be skipped." ) {
		CHECK( skipped_hspace("\t.\t ", 1) );
		CHECK( skipped_hspace("\t(\t ", 1) );
		CHECK( skipped_hspace("\tabc \t", 1) );
		CHECK( skipped_hspace("\t123 \t", 1) );
		CHECK( skipped_hspace("\t\r\t ", 1) );
		CHECK( skipped_hspace("\t\n\t ", 1) );

		CHECK( skipped_hspace("\t\t\t. \t", 3) );
		CHECK( skipped_hspace("\t\t\t( \t", 3) );
		CHECK( skipped_hspace("\t\t\tabc\t ", 3) );
		CHECK( skipped_hspace("\t\t\t123\t ", 3) );
		CHECK( skipped_hspace("\t\t\t\r\t ", 3) );
		CHECK( skipped_hspace("\t\t\t\n\t ", 3) );

		CHECK_FALSE( skipped_hspace(". \t ", 1) );
		CHECK_FALSE( skipped_hspace("() \t ", 1) );
		CHECK_FALSE( skipped_hspace("abc\t ", 1) );
		CHECK_FALSE( skipped_hspace("123\t ", 1) );
	}
}

TEST_CASE( "rush::skip_vspace", "[unit][lexer]" ) {

	SECTION( "leading carriage-return characters should be skipped." ) {
		// carriage-return
		CHECK( skipped_vspace("\r.\n\r\v", 1) );
		CHECK( skipped_vspace("\r(\n\r\v", 1) );
		CHECK( skipped_vspace("\rabc\r\n\v", 1) );
		CHECK( skipped_vspace("\r123\r\n\v", 1) );
		CHECK( skipped_vspace("\r \n\r", 1) );
		CHECK( skipped_vspace("\r\t\n\r", 1) );

		CHECK( skipped_vspace("\r\r\r.\n\r\v", 3) );
		CHECK( skipped_vspace("\r\r\r(\n\r\v", 3) );
		CHECK( skipped_vspace("\r\r\rabc\r\n\v", 3) );
		CHECK( skipped_vspace("\r\r\r123\r\n\v", 3) );
		CHECK( skipped_vspace("\r\r\r \n\r\v", 3) );
		CHECK( skipped_vspace("\r\r\r\t\n\r\v", 3) );

		CHECK_FALSE( skipped_vspace(".\n\r\v", 1) );
		CHECK_FALSE( skipped_vspace("()\n\r\v", 1) );
		CHECK_FALSE( skipped_vspace("abc\r\n\v", 1) );
		CHECK_FALSE( skipped_vspace("123\r\n\v", 1) );
	}

	SECTION( "leading line-feed characters should be skipped" ) {
		CHECK( skipped_vspace("\n.\n\r\v", 1) );
		CHECK( skipped_vspace("\n(\n\r\v", 1) );
		CHECK( skipped_vspace("\nabc\r\n\v", 1) );
		CHECK( skipped_vspace("\n123\r\n\v", 1) );
		CHECK( skipped_vspace("\n \n\r", 1) );
		CHECK( skipped_vspace("\n\t\n\r", 1) );

		CHECK( skipped_vspace("\n\n\n.\n\r\v", 3) );
		CHECK( skipped_vspace("\n\n\n(\n\r\v", 3) );
		CHECK( skipped_vspace("\n\n\nabc\r\n\v", 3) );
		CHECK( skipped_vspace("\n\n\n123\r\n\v", 3) );
		CHECK( skipped_vspace("\n\n\n \n\r\v", 3) );
		CHECK( skipped_vspace("\n\n\n\t\n\r\v", 3) );

		CHECK_FALSE( skipped_vspace(".\n\r\v", 1) );
		CHECK_FALSE( skipped_vspace("()\n\r\v", 1) );
		CHECK_FALSE( skipped_vspace("abc\r\n\v", 1) );
		CHECK_FALSE( skipped_vspace("123\r\n\v", 1) );
	}
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
