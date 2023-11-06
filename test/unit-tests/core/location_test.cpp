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
