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
#include "catch2/catch.hpp"
#include "rush/extra/iterator.hpp"

#include <array>

TEST_CASE( "rush::iterators::advance_if", "[unit][core]" ) {

	GIVEN( "an empty range" ) {
		std::array<int, 0> a;
		auto const last = std::end(a);
		auto first = std::begin(a);
		auto pos = first;

		WHEN( "advancing over the range" ) {
			bool result = rush::advance_if(pos, last, [](auto& x) { return true; });

			THEN( "return value should be false" ) { CHECK_FALSE( result ); }
			THEN( "first iterator should not be changed" ) { CHECK(pos == first); }
		}
	}

	GIVEN( "a range of elements" ) {
		std::array<int, 3> a { 1, 2, 3 };
		auto const last = std::end(a);
		auto first = std::begin(a);
		auto pos = first;

		WHEN( "the predicate returns false" ) {
			rush::advance_if(pos, last, [](auto& x) { return false; });

			THEN( "the referenced iterator should not advance") { CHECK( pos == first ); }
		}

		WHEN( "the predicate returns true" ) {
			rush::advance_if(pos, last, [](auto& x) { return true; });

			THEN( "iterator should advance one past the last element" ) { CHECK( pos == last ); }
		}

		SECTION("should advance the iterator while the predicate is true") {
			rush::advance_if(pos, last, [](auto& x) { return x < 2; });
			REQUIRE( *pos == 2 );
		}
	}
}
