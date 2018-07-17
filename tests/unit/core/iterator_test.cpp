#include "catch2/catch.hpp"
#include "rush/core/iterator.hpp"


TEST_CASE( "iterator advance if", "[unit][core]" ) {
	std::vector<int> v { 1, 2, 3 };
	auto const last = std::end(v);
	auto first = std::begin(v);

	SECTION("should not advance past the end") {
		rush::advance_if(first, last, [](auto& x) { return true; });
		REQUIRE( first == last );
	}

	SECTION("should advance the iterator while the predicate is true") {
		rush::advance_if(first, last, [](auto& x) { return x < 2; });
		REQUIRE( *first == 2 );
	}

	SECTION("should not advance the iterator while the predicate is false") {
		rush::advance_if(first, last, [](auto& x) { return false; });
		REQUIRE( first == first );
	}
}
