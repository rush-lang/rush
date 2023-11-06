/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#include <catch2/catch_all.hpp>
#include "rush/core/ascii.hpp"


using namespace rush::ascii;

TEST_CASE( "rush::ascii::is_letter", "[unit][core]" ) {
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


TEST_CASE( "rush::ascii::is_digit", "[unit][core]" ) {
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

TEST_CASE( "rush::ascii::is_zero_digit", "[unit][core]" ) {
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

TEST_CASE( "rush::ascii::is_space", "[unit][core]" ) {
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


TEST_CASE( "rush::ascii::is_ident_head", "[unit][core]" ) {
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


TEST_CASE( "rush::ascii::is_ident_body", "[unit][core]" ) {
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

TEST_CASE ("rush::ascii::iequal", "[unit][core]" ) {
	CHECK( iequal('a', 'a') );
	CHECK( iequal('A', 'A') );
	CHECK( iequal('z', 'z') );
	CHECK( iequal('Z', 'Z') );
	CHECK( iequal('0', '0') );
	CHECK( iequal('.', '.') );

	CHECK( iequal('a', 'A') );
	CHECK( iequal('A', 'a') );
	CHECK( iequal('z', 'Z') );
	CHECK( iequal('Z', 'z') );

	CHECK_FALSE( iequal('a', 'z') );
	CHECK_FALSE( iequal('z', 'a') );

	CHECK_FALSE( iequal('a', 'Z') );
	CHECK_FALSE( iequal('z', 'A') );
}
