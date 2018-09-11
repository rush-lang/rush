#include "catch2/catch.hpp"
#include "rush/lexer/lex.hpp"
#include "rush/lexer/dump.hpp"

#include <string_view>
#include <initializer_list>
#include <vector>

namespace tok = rush::tokens;
namespace symbols = rush::symbols;

using rush::lexical_token_prefix;
using rush::lexical_token_suffix;


bool valid_lex(std::string input, std::vector<rush::lexical_token> expect) {
	auto lxa = rush::lex(input);
	auto result = lxa.size() == expect.size() && std::equal(
		lxa.begin(), lxa.end(), expect.begin(), expect.end());
	if (!result) rush::dump(lxa);
	return result;
}

bool valid_lex_same(std::string input, std::vector<rush::lexical_token> expect) {
	auto lxa = rush::lex(input);
	auto result = lxa.size() == expect.size() && std::equal(
		lxa.begin(), lxa.end(), expect.begin(), expect.end(),
		[](auto& lhs, auto& rhs) { return lhs.is_same(rhs); });
	if (!result) rush::dump(lxa);
	return result;
}

char const* add_func_str =
R"(func add(a, b: numeric):
	return a + b
)";

char const* map_xs_str =
R"(let xs = map(1...10, pow($, 2)))";

char const* hello_world_str =
R"(import std.io

func main(args: string[]):
	println("hello world!")
)";

TEST_CASE( "rush::lex" ) {

	CHECK( valid_lex(add_func_str, {
		tok::func_keyword({ 1, 1 }),
		tok::identifier("add", { 1, 6 }),
		tok::left_parenthesis({ 1, 9 }),
		tok::identifier("a", { 1, 10 }),
		tok::comma({ 1, 11 }),
		tok::identifier("b", { 1, 13 }),
		tok::colon({ 1, 14 }),
		tok::identifier("numeric", { 1, 16 }),
		tok::right_parenthesis({ 1, 23 }),
		tok::colon({ 1, 24 }),
		tok::indent({ 2, 1 }),
		tok::return_keyword({ 2, 2 }),
		tok::identifier("a", { 2, 9 }),
		tok::plus({ 2, 11 }),
		tok::identifier("b", { 2, 13 }),
		tok::dedent({ 3, 1 }),
	}));

	CHECK( valid_lex(map_xs_str, {
		tok::let_keyword({ 1, 1 }),
		tok::identifier("xs", { 1, 5 }),
		tok::equals({ 1, 8 }),
		tok::identifier("map", { 1, 10 }),
		tok::left_parenthesis({ 1, 13 }),
		tok::integer_literal(1, { 1, 14 }),
		tok::ellipses({ 1, 15 }),
		tok::integer_literal(10, { 1, 18 }),
		tok::comma({ 1, 20 }),
		tok::identifier("pow", { 1, 22 }),
		tok::left_parenthesis({ 1, 25 }),
		tok::dollar({ 1, 26 }),
		tok::comma({ 1, 27 }),
		tok::integer_literal(2, { 1, 29 }),
		tok::right_parenthesis({ 1, 30 }),
		tok::right_parenthesis({ 1, 31 }),
	}));

	CHECK( valid_lex_same(hello_world_str, {
		tok::import_keyword(),
		tok::identifier("std"),
		tok::period(),
		tok::identifier("io"),
		tok::func_keyword(),
		tok::identifier("main"),
		tok::left_parenthesis(),
		tok::identifier("args"),
		tok::colon(),
		tok::string_keyword(),
		tok::left_square_bracket(),
		tok::right_square_bracket(),
		tok::right_parenthesis(),
		tok::colon(),
		tok::indent(),
		tok::identifier("println"),
		tok::left_parenthesis(),
		tok::string_literal("hello world!"),
		tok::right_parenthesis(),
		tok::dedent(),
	}));
}

TEST_CASE( "rush::lex (symbols)", "[unit][lexer]" ) {

	SECTION( "ellipses" ) {
		CHECK( valid_lex("...", { tok::ellipses({ 1, 1 }) }));

		CHECK( valid_lex("1...10", {
			tok::integer_literal(1, { 1, 1 }),
			tok::ellipses({ 1, 2 }),
			tok::integer_literal(10, { 1, 5 }),
		}));

		CHECK( valid_lex("x...y", {
			tok::identifier("x", { 1, 1 }),
			tok::ellipses({ 1, 2 }),
			tok::identifier("y", { 1, 5 }),
		}));
	}
}

TEST_CASE( "rush::lex (keywords)", "[unit][lexer]" ) {

	CHECK( valid_lex("if else while", {
		tok::if_keyword({ 1, 1 }),
		tok::else_keyword({ 1, 4 }),
		tok::while_keyword({ 1, 9 })
	}));

	CHECK_FALSE( valid_lex("_let", { tok::let_keyword({ 1, 1 }) }) );
	CHECK_FALSE( valid_lex("var_", { tok::var_keyword({ 1, 1 }) }) );
	CHECK_FALSE( valid_lex("const1", { tok::const_keyword({ 1, 1 }) }) );
}

TEST_CASE( "rush::lex (identifiers)", "[unit][lexer]" ) {

	CHECK( valid_lex("a z A Z abc XYZ", {
		tok::identifier("a", { 1, 1 }),
		tok::identifier("z", { 1, 3 }),
		tok::identifier("A", { 1, 5 }),
		tok::identifier("Z", { 1, 7 }),
		tok::identifier("abc", { 1, 9 }),
		tok::identifier("XYZ", { 1, 13 }),
	}));

	CHECK( valid_lex("_ _a _z _0 _9 _a0 _z0 _a9 _z9", {
		tok::identifier("_", { 1, 1 }),
		tok::identifier("_a", { 1, 3 }),
		tok::identifier("_z", { 1, 6 }),
		tok::identifier("_0", { 1, 9 }),
		tok::identifier("_9", { 1, 12 }),
		tok::identifier("_a0", { 1, 15 }),
		tok::identifier("_z0", { 1, 19 }),
		tok::identifier("_a9", { 1, 23 }),
		tok::identifier("_z9", { 1, 27 }),
	}));

	CHECK( valid_lex("a1 c_ d2_ e_3 f4_ab12__", {
		tok::identifier("a1", { 1, 1 }),
		tok::identifier("c_", { 1, 4 }),
		tok::identifier("d2_", { 1, 7 }),
		tok::identifier("e_3", { 1, 11 }),
		tok::identifier("f4_ab12__", { 1, 15 }),
	}));

	// like-joined
	CHECK( valid_lex("abc123___ abc___123 ___abc123 ___123abc", {
		tok::identifier("abc123___", { 1, 1 }),
		tok::identifier("abc___123", { 1, 11 }),
		tok::identifier("___abc123", { 1, 21 }),
		tok::identifier("___123abc", { 1, 31 }),
	}));

	// like-interspersed
	CHECK( valid_lex("a_1b_2c_3 _a1_b2_c3 _1a_2b_3c", {
		tok::identifier("a_1b_2c_3", { 1, 1 }),
		tok::identifier("_a1_b2_c3", { 1, 11 }),
		tok::identifier("_1a_2b_3c", { 1, 21 }),
	}));
}

TEST_CASE( "rush::lex (floating literals)", "[unit][lexer]" ) {
	CHECK( valid_lex("1.0", { tok::floating_literal(1.0, { 1, 1 }) }) );
	CHECK( valid_lex("9.0", { tok::floating_literal(9.0, { 1, 1 }) }) );
	CHECK( valid_lex(".0", { tok::floating_literal(0.0, { 1, 1 }) }) );
	CHECK( valid_lex(".013", { tok::floating_literal(0.013, { 1, 1 }) }) );
	CHECK( valid_lex("0.0", { tok::floating_literal(0.0, { 1, 1 }) }) );
	CHECK( valid_lex("123.", { tok::floating_literal(123.0, { 1, 1 }) }) );
	CHECK_FALSE( valid_lex("0", { tok::floating_literal(0, { 1, 1 }) }) );
	CHECK_FALSE( valid_lex("1", { tok::floating_literal(1, { 1, 1 }) }) );
	CHECK_FALSE( valid_lex("9", { tok::floating_literal(9, { 1, 1 }) }) );
	CHECK_FALSE( valid_lex("123", { tok::floating_literal(123, { 1, 1 }) }) );

	CHECK( valid_lex("1.0f", { tok::suffixed_floating_literal(1.0, lexical_token_suffix::float_literal, { 1, 1 }) }) );
	CHECK( valid_lex("9.0f", { tok::suffixed_floating_literal(9.0, lexical_token_suffix::float_literal, { 1, 1 }) }) );
	CHECK( valid_lex(".0f", { tok::suffixed_floating_literal(0.0, lexical_token_suffix::float_literal, { 1, 1 }) }) );
	CHECK( valid_lex(".013f", { tok::suffixed_floating_literal(0.013, lexical_token_suffix::float_literal, { 1, 1 }) }) );
	CHECK( valid_lex("0.f", { tok::suffixed_floating_literal(0.0, lexical_token_suffix::float_literal, { 1, 1 }) }) );
	CHECK( valid_lex("123.f", { tok::suffixed_floating_literal(123.0, lexical_token_suffix::float_literal, { 1, 1 }) }) );

	CHECK_FALSE( valid_lex("1.0f", { tok::suffixed_floating_literal(1.0, lexical_token_suffix::none, { 1, 1 }) }) );
	CHECK_FALSE( valid_lex("1.0", { tok::suffixed_floating_literal(1.0, lexical_token_suffix::float_literal, { 1, 1 }) }) );
}

TEST_CASE( "rush::lex (integer literals)", "[unit][lexer]" ) {

	CHECK( valid_lex("0 1 9 10 1234567890 9876543210", {
		tok::integer_literal(0, { 1, 1 }),
		tok::integer_literal(1, { 1, 3 }),
		tok::integer_literal(9, { 1, 5 }),
		tok::integer_literal(10, { 1, 7 }),
		tok::integer_literal(1234567890LL, { 1, 10 }),
		tok::integer_literal(9876543210LL, { 1, 21 }),
	}));

	CHECK( valid_lex("0u 1u 9u 10u 1234567890u 9876543210u", {
		tok::suffixed_integer_literal(0, lexical_token_suffix::unsigned_literal, { 1, 1 }),
		tok::suffixed_integer_literal(1, lexical_token_suffix::unsigned_literal, { 1, 4 }),
		tok::suffixed_integer_literal(9, lexical_token_suffix::unsigned_literal, { 1, 7 }),
		tok::suffixed_integer_literal(10, lexical_token_suffix::unsigned_literal, { 1, 10 }),
		tok::suffixed_integer_literal(1234567890LL, lexical_token_suffix::unsigned_literal, { 1, 14 }),
		tok::suffixed_integer_literal(9876543210LL, lexical_token_suffix::unsigned_literal, { 1, 26 }),
	}));

	CHECK( valid_lex("0l 1l 9l 10l 1234567890l 9876543210l", {
		tok::suffixed_integer_literal(0, lexical_token_suffix::long_literal, { 1, 1 }),
		tok::suffixed_integer_literal(1, lexical_token_suffix::long_literal, { 1, 4 }),
		tok::suffixed_integer_literal(9, lexical_token_suffix::long_literal, { 1, 7 }),
		tok::suffixed_integer_literal(10, lexical_token_suffix::long_literal, { 1, 10 }),
		tok::suffixed_integer_literal(1234567890LL, lexical_token_suffix::long_literal, { 1, 14 }),
		tok::suffixed_integer_literal(9876543210LL, lexical_token_suffix::long_literal, { 1, 26 }),
	}));

	CHECK( valid_lex("0x01 0x09 0X0e 0x0F 0x0a 0X0B 0xdeadbeef", {
		tok::integer_literal(0x01, { 1, 1 }),
		tok::integer_literal(0x09, { 1, 6 }),
		tok::integer_literal(0x0e, { 1, 11 }),
		tok::integer_literal(0x0F, { 1, 16 }),
		tok::integer_literal(0x0a, { 1, 21 }),
		tok::integer_literal(0x0B, { 1, 26 }),
		tok::integer_literal(0xdeadbeef, { 1, 31 }),
	}));

	CHECK( valid_lex("0o10", { tok::integer_literal(010, { 1, 1 }), }));
	CHECK( valid_lex("0b00101010", { tok::integer_literal(42, { 1, 1 }) }));


	CHECK_FALSE( valid_lex("123u", { tok::suffixed_integer_literal(123, lexical_token_suffix::none, { 1, 1 }) }) );
	CHECK_FALSE( valid_lex("123l", { tok::suffixed_integer_literal(123, lexical_token_suffix::none, { 1, 1 }) }) );

	CHECK_FALSE( valid_lex("123u", { tok::suffixed_integer_literal(123, lexical_token_suffix::long_literal, { 1, 1 }) }) );
	CHECK_FALSE( valid_lex("123l", { tok::suffixed_integer_literal(123, lexical_token_suffix::unsigned_literal, { 1, 1 }) }) );

	// any leading-zero or non-digit should stop the scan,
	// and proceed to scan the next character as a seperate token.
	CHECK( valid_lex("01", { tok::integer_literal(0, { 1, 1 }), tok::integer_literal(1, { 1, 2 }) }) );
	CHECK( valid_lex("09", { tok::integer_literal(0, { 1, 1 }), tok::integer_literal(9, { 1, 2 }) }) );
	CHECK( valid_lex("0123456789", { tok::integer_literal(0, { 1, 1 }), tok::integer_literal(123456789LL, { 1, 2 }) }) );
	CHECK( valid_lex("0987654321", { tok::integer_literal(0, { 1, 1 }), tok::integer_literal(987654321LL, { 1, 2 }) }) );
	CHECK( valid_lex("0_", { tok::integer_literal(0, { 1, 1 }), tok::identifier("_", { 1, 2 }) }) );
	CHECK( valid_lex("1a", { tok::integer_literal(1, { 1, 1 }), tok::identifier("a", { 1, 2 }) }) );
	CHECK( valid_lex("123_", { tok::integer_literal(123, { 1, 1 }), tok::identifier("_", { 1, 4 }) }) );
	CHECK( valid_lex("123a", { tok::integer_literal(123, { 1, 1 }), tok::identifier("a", { 1, 4 }) }) );
	CHECK( valid_lex("1.0", { tok::floating_literal(1.0, { 1, 1 }) }) );
	CHECK( valid_lex("123.9", { tok::floating_literal(123.9, { 1, 1 }) }) );

	// non-digit characters directly preceding a
	// digit should generate seperate tokens.
	CHECK( valid_lex("_0", { tok::identifier("_0", { 1, 1 }) }) );
	CHECK( valid_lex("a1", { tok::identifier("a1", { 1, 1 }) }) );
	CHECK( valid_lex("-1", { tok::minus({ 1, 1 }), tok::integer_literal(1, { 1, 2 }) }) );
	CHECK( valid_lex("+1", { tok::plus({ 1, 1 }), tok::integer_literal(1, { 1, 2 }) }) );
}

TEST_CASE( "rush::lex (string literal)", "[unit][lexer]" ) {
	CHECK( valid_lex("\"\"", { tok::string_literal("", { 1, 1 }) }) );
	CHECK( valid_lex("\"a\"", { tok::string_literal("a", { 1, 1 }) }) );
	CHECK( valid_lex("\"abc\"", { tok::string_literal("abc", { 1, 1 })} ) );
	CHECK( valid_lex("\"0\"", { tok::string_literal("0", { 1, 1 }) }) );
	CHECK( valid_lex("\"123\"", { tok::string_literal("123", { 1, 1 }) }) );
	CHECK( valid_lex("\"\\\"\"", { tok::string_literal("\\\"", { 1, 1 }) }) );
	CHECK( valid_lex("\"!\\£$%^&*(){}[]-=_+:;@~/?.,<>|\\\\\"", { tok::string_literal("!\\£$%^&*(){}[]-=_+:;@~/?.,<>|\\\\", { 1, 1 }) }) );
}

TEST_CASE( "rush::lex (tab-indentation)" ) {
	CHECK( valid_lex("\tabc", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::dedent({ 2, 1 }),
	}));

	CHECK( valid_lex("\tabc\ndef", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::dedent({ 2, 1 }),
		tok::identifier("def", { 2, 1 }),
	}));

	CHECK( valid_lex("\tabc\n\t\tdef", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::indent({ 2, 1 }),
		tok::identifier("def", { 2, 3 }),
		tok::dedent({ 3, 2 }),
		tok::dedent({ 4, 1 }),
	}));

	CHECK( valid_lex("\tabc\n\t\tdef\n\tghi", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::indent({ 2, 1 }),
		tok::identifier("def", { 2, 3 }),
		tok::dedent({ 3, 1 }),
		tok::identifier("ghi", { 3, 2 }),
		tok::dedent({ 4, 1 }),
	}));

	// initial indentation space (tab/spaces) is used to discover
	// the indentation width. After the initial, subsequent indents
	// require the same number of spaces or tabs.
	CHECK( valid_lex("\t\tabc", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 3 }),
		tok::dedent({ 2, 1 }),
	}));

	CHECK( valid_lex("\t\tabc\n\tdef", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 3 }),
		tok::dedent({ 2, 1 }),
		tok::identifier("def", { 2, 2 })
	}));

	CHECK( valid_lex("\tabc\n\tdef", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::identifier("def", { 2, 2 }),
		tok::dedent({ 3, 1 })
	}));

	// empty lines should not produce indents/dedents
	CHECK( valid_lex("\t", { }) );
	CHECK( valid_lex("\n\t", { }) );

	CHECK( valid_lex("\tabc\n\t  \n\tdef", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::identifier("def", { 3, 2 }),
		tok::dedent({ 4, 1 })
	}));

	CHECK( valid_lex("\tabc\n\t\t\n\tdef", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::identifier("def", { 3, 2 }),
		tok::dedent({ 4, 1 })
	}));

	CHECK( valid_lex("\tabc\n\t\tdef\nghi", {
		tok::indent({ 1, 1 }),
		tok::identifier("abc", { 1, 2 }),
		tok::indent({ 2, 1 }),
		tok::identifier("def", { 2, 3 }),
		tok::dedent({ 3, 1 }),
		tok::dedent({ 3, 1 }),
		tok::identifier("ghi", { 3, 1 }),
	}));
}
