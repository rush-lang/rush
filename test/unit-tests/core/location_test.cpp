#include "catch2/catch.hpp"
#include "rush/core/location.hpp"

using rush::location;

bool is_location_equal(location const& lhs, location const& rhs) { return lhs == rhs; }
bool is_location_less(location const& lhs, location const& rhs) { return lhs < rhs; }

TEST_CASE( "rush::location", "[unit][core]" ) {

	SECTION( "should hold line and column" ) {
		auto loc = location { 1, 2 };
		REQUIRE( loc.line() == 1 );
		REQUIRE( loc.column() == 2 );
	}

	SECTION( "location should be less than another based on line and column" ) {
		CHECK_FALSE( is_location_less({ 1, 1 }, { 1, 1 }) );
		CHECK( is_location_less({ 1, 1 }, { 1, 2 }) );
		CHECK( is_location_less({ 1, 1 }, { 2, 1 }) );
		CHECK( is_location_less({ 1, 1 }, { 2, 2 }) );
	}

	SECTION( "location should be equal to another based on line and column" ) {
		CHECK_FALSE( is_location_equal({ 1, 1 }, { 1, 2 }) );
		CHECK_FALSE( is_location_equal({ 1, 1 }, { 2, 1 }) );
		CHECK( is_location_equal({ 1, 1 }, { 1, 1 }) );
		CHECK( is_location_equal({ 2, 2 }, { 2, 2 }) );
	}
}