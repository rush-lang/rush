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
		CHECK( void_keyword().text() == "void" );
		CHECK( identifier("abc").text() == "abc" );
		CHECK( string_literal("def").text() == "def" );
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
