#include "catch.hpp"
#include "rush/core/charinfo.hpp"


using namespace rush::charinfo;

TEST_CASE( "letter characters", "[unit][core]" ) {
	REQUIRE_FALSE( is_letter('0') );
	REQUIRE_FALSE( is_letter('9') );

	REQUIRE( is_letter('a') );
	REQUIRE( is_letter('A') );
	REQUIRE( is_letter('z') );
	REQUIRE( is_letter('Z') );

	REQUIRE_FALSE( is_letter('.') );
	REQUIRE_FALSE( is_letter('_') );

	REQUIRE_FALSE( is_letter(0x0) );
	REQUIRE_FALSE( is_letter(0xffffff) );

	REQUIRE_FALSE( is_letter('\x80') );
	REQUIRE_FALSE( is_letter('\xc2') );
	REQUIRE_FALSE( is_letter('\xff') );
}


TEST_CASE( "digit characters", "[unit][core]" ) {
	REQUIRE_FALSE( is_digit('a') );
	REQUIRE_FALSE( is_digit('A') );
	REQUIRE_FALSE( is_digit('z') );
	REQUIRE_FALSE( is_digit('Z') );

	REQUIRE( is_digit('0') );
	REQUIRE( is_digit('1') );
	REQUIRE( is_digit('8') );
	REQUIRE( is_digit('9') );

	REQUIRE_FALSE( is_digit('.') );
	REQUIRE_FALSE( is_digit('_') );

	REQUIRE_FALSE( is_digit('\x80') );
	REQUIRE_FALSE( is_digit('\xc2') );
	REQUIRE_FALSE( is_digit('\xff') );
}


TEST_CASE( "white space characters", "[unit][core]" ) {
	REQUIRE_FALSE( is_space('a') );
	REQUIRE_FALSE( is_space('_') );
	REQUIRE_FALSE( is_space('0') );
	REQUIRE_FALSE( is_space('.') );
	REQUIRE_FALSE( is_space('\0') );
	REQUIRE_FALSE( is_space('\x7f') );

	REQUIRE( is_space(' ') );
	REQUIRE( is_space('\t') );
	REQUIRE( is_space('\f') );
	REQUIRE( is_space('\v') );

	REQUIRE( is_space('\n') );
	REQUIRE( is_space('\r') );

	REQUIRE_FALSE( is_space('\x80') );
	REQUIRE_FALSE( is_space('\xc2') );
	REQUIRE_FALSE( is_space('\xff') );
}


TEST_CASE( "identifier head characters", "[unit][core]" ) {
	REQUIRE_FALSE( is_ident_head('.') );
	REQUIRE_FALSE( is_ident_head('0') );
	REQUIRE_FALSE( is_ident_head('9') );

	REQUIRE( is_ident_head('_') );
	REQUIRE( is_ident_head('a') );
	REQUIRE( is_ident_head('A') );
	REQUIRE( is_ident_head('z') );
	REQUIRE( is_ident_head('Z') );

	REQUIRE_FALSE( is_ident_head(0x0) );
	REQUIRE_FALSE( is_ident_head(0xffffff) );

	REQUIRE_FALSE( is_ident_head('\x80') );
	REQUIRE_FALSE( is_ident_head('\xc2') );
	REQUIRE_FALSE( is_ident_head('\xff') );
}


TEST_CASE( "identifier body characters", "[unit][core]" ) {
	REQUIRE_FALSE( is_ident_body('.') );

	REQUIRE( is_ident_body('0') );
	REQUIRE( is_ident_body('9') );

	REQUIRE( is_ident_body('_') );
	REQUIRE( is_ident_body('a') );
	REQUIRE( is_ident_body('A') );
	REQUIRE( is_ident_body('z') );
	REQUIRE( is_ident_body('Z') );

	REQUIRE_FALSE( is_ident_body(0x0) );
	REQUIRE_FALSE( is_ident_body(0xffffff) );

	REQUIRE_FALSE( is_ident_body('\x80') );
	REQUIRE_FALSE( is_ident_body('\xc2') );
	REQUIRE_FALSE( is_ident_body('\xff') );
}