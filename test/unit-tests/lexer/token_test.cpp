#include "catch2/catch.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"

namespace rtok = rush::tokens;

TEST_CASE( "rush::lexical_token", "[unit][lexer]" ) {
	SECTION( "symbols" ) {
		auto tok = rtok::eof();
		// REQUIRE( tok.is_symbol() );
		// REQUIRE( tok.is( rush::symbols::eof ) );
		// REQUIRE( tok.type() == rush::lexical_token_type::symbol );
	}

	SECTION( "keywords" ) {

	}
}