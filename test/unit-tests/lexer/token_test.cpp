#include "catch2/catch.hpp"
#include "rush/lexer/token.hpp"

#include <iostream>

using namespace rush::tokens;
namespace symbols = rush::symbols;
namespace keywords = rush::keywords;

TEST_CASE( "rush::lexical_token", "[unit][lexer]" ) {

	SECTION( "symbols" ) {

	}

	SECTION( "keywords" ) {
		CHECK( if_keyword().is(keywords::if_) );
		CHECK( else_keyword().is(keywords::else_) );
		CHECK( while_keyword().is(keywords::while_) );
	}

	SECTION( "rush::lexical_token::text" ) {
		CHECK( identifier("abc").text() == "abc" );
		CHECK( void_keyword().text() == "void" );
		CHECK( integer_literal(123).text() == "123" );

		// todo: should probably do something about the formatting of
		// decimals. not that important though as lexical_token::text()
		// is not expected to be used outside of diagnostics.
		CHECK( floating_literal(123.321).text() == "123.321000" );
	}

	SECTION( "rush::lexical_token::is_any" ) {
		CHECK( let_keyword().is_any(keywords::let_) );
		CHECK( let_keyword().is_any(symbols::plus, keywords::let_, symbols::minus) );
		CHECK( let_keyword().is_any(symbols::plus, symbols::minus, keywords::let_) );

		CHECK_FALSE( let_keyword().is_any(keywords::if_, symbols::plus, symbols::minus) );
		CHECK_FALSE( let_keyword().is_any(symbols::plus, keywords::if_, symbols::minus) );
		CHECK_FALSE( let_keyword().is_any(symbols::plus, symbols::minus, keywords::if_) );
	}

	SECTION( "rush::lexical_token::is_not_any" ) {
		CHECK( var_keyword().is_not_any(keywords::try_) );
		CHECK( var_keyword().is_not_any(symbols::indent, keywords::catch_, symbols::dedent) );
		CHECK( var_keyword().is_not_any(symbols::indent, symbols::dedent, keywords::finally_) );

		CHECK_FALSE( var_keyword().is_not_any(keywords::var_, symbols::indent, symbols::dedent) );
		CHECK_FALSE( var_keyword().is_not_any(symbols::indent, keywords::var_, symbols::dedent) );
		CHECK_FALSE( var_keyword().is_not_any(symbols::indent, symbols::dedent, keywords::var_) );
	}
}