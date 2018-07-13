#include "catch.hpp"
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

TEST_CASE( "iterator next if", "[unit][core]" ) {
	std::vector<int> v { 1, 2, 3 };
	auto const first = std::begin(v);
	auto const last = std::end(v);

	SECTION("should not advance past the end") {
		auto it = rush::next_if(first, last, [](auto& x) { return true; });
		REQUIRE( it == last );
	}

	SECTION("should advance the iterator while the predicate is true") {
		auto it = rush::next_if(first, last, [](auto& x) { return x < 2; });
		REQUIRE( *it == 2 );
	}

	SECTION("should not advance the iterator while the predicate is false") {
		auto it = rush::next_if(first, last, [](auto& x) { return false; });
		REQUIRE( it == first );
	}
}