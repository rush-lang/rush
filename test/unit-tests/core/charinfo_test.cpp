#include "catch2/catch.hpp"
#include "rush/core/charinfo.hpp"


using namespace rush::charinfo;

TEST_CASE( "rush::charinfo::is_letter", "[unit][core]" ) {
	CHECK_FALSE( is_letter('0') );
	CHECK_FALSE( is_letter('9') );

	CHECK( is_letter('a') );
	CHECK( is_letter('A') );
	CHECK( is_letter('z') );
	CHECK( is_letter('Z') );

	CHECK_FALSE( is_letter('.') );
	CHECK_FALSE( is_letter('_') );

	// FIXME: Currently causing segfault.
	// CHECK_FALSE( is_letter(0x0) );
	// CHECK_FALSE( is_letter(0xffffff) );

	CHECK_FALSE( is_letter('\x80') );
	CHECK_FALSE( is_letter('\xc2') );
	CHECK_FALSE( is_letter('\xff') );
}


TEST_CASE( "rush::charinfo::is_digit", "[unit][core]" ) {
	CHECK_FALSE( is_digit('a') );
	CHECK_FALSE( is_digit('A') );
	CHECK_FALSE( is_digit('z') );
	CHECK_FALSE( is_digit('Z') );

	CHECK( is_digit('0') );
	CHECK( is_digit('1') );
	CHECK( is_digit('8') );
	CHECK( is_digit('9') );

	CHECK_FALSE( is_digit('.') );
	CHECK_FALSE( is_digit('_') );

	CHECK_FALSE( is_digit('\x80') );
	CHECK_FALSE( is_digit('\xc2') );
	CHECK_FALSE( is_digit('\xff') );
}

TEST_CASE( "rush::charinfo::is_zero_digit", "[unit][core]" ) {
	CHECK_FALSE( is_non_zero_digit('0') );

	CHECK( is_non_zero_digit('1') );
	CHECK( is_non_zero_digit('9') );

	CHECK_FALSE( is_non_zero_digit('a') );
	CHECK_FALSE( is_non_zero_digit('A') );
	CHECK_FALSE( is_non_zero_digit('z') );
	CHECK_FALSE( is_non_zero_digit('Z') );

	CHECK_FALSE( is_non_zero_digit('.') );
	CHECK_FALSE( is_non_zero_digit('_') );
}

TEST_CASE( "rush::charinfo::is_space", "[unit][core]" ) {
	CHECK_FALSE( is_space('a') );
	CHECK_FALSE( is_space('_') );
	CHECK_FALSE( is_space('0') );
	CHECK_FALSE( is_space('.') );
	CHECK_FALSE( is_space('\0') );
	CHECK_FALSE( is_space('\x7f') );

	CHECK( is_space(' ') );
	CHECK( is_space('\t') );
	CHECK( is_space('\v') );
	// CHECK( is_space('\f') );

	CHECK( is_space('\n') );
	CHECK( is_space('\r') );

	CHECK_FALSE( is_space('\x80') );
	CHECK_FALSE( is_space('\xc2') );
	CHECK_FALSE( is_space('\xff') );
}


TEST_CASE( "identifier head characters", "[unit][core]" ) {
	CHECK_FALSE( is_ident_head('.') );
	CHECK_FALSE( is_ident_head('0') );
	CHECK_FALSE( is_ident_head('9') );

	CHECK( is_ident_head('_') );
	CHECK( is_ident_head('a') );
	CHECK( is_ident_head('A') );
	CHECK( is_ident_head('z') );
	CHECK( is_ident_head('Z') );

	// FIXME: Currently causing segfault.
	// CHECK_FALSE( is_ident_head(0x0) );
	// CHECK_FALSE( is_ident_head(0xffffff) );

	CHECK_FALSE( is_ident_head('\x80') );
	CHECK_FALSE( is_ident_head('\xc2') );
	CHECK_FALSE( is_ident_head('\xff') );
}


TEST_CASE( "identifier body characters", "[unit][core]" ) {
	CHECK_FALSE( is_ident_body('.') );

	CHECK( is_ident_body('0') );
	CHECK( is_ident_body('9') );

	CHECK( is_ident_body('_') );
	CHECK( is_ident_body('a') );
	CHECK( is_ident_body('A') );
	CHECK( is_ident_body('z') );
	CHECK( is_ident_body('Z') );

	// FIXME: Currently causing segfault.
	// CHECK_FALSE( is_ident_body(0x0) );
	// CHECK_FALSE( is_ident_body(0xffffff) );

	CHECK_FALSE( is_ident_body('\x80') );
	CHECK_FALSE( is_ident_body('\xc2') );
	CHECK_FALSE( is_ident_body('\xff') );
}