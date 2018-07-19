#include "catch2/catch.hpp"
#include "rush/core/iterator.hpp"

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
