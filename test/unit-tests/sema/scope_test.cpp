#include "catch2/catch.hpp"
#include "rush/sema/scope.hpp"

TEST_CASE( "rush::scope", "[unit][sema]" ) {

	SECTION( "nesting scopes" ) {

		auto& gs = rush::global_scope;
		REQUIRE( gs.depth() == 0 );
		REQUIRE( gs.parent() == nullptr );
		REQUIRE( gs.is_global() );
		REQUIRE_FALSE( gs.is_local_to(gs) );

		auto& s1 = gs.push_scope();
		REQUIRE( s1.depth() == 1 );
		REQUIRE( s1.parent() == &gs );
		REQUIRE( s1.is_local_to(gs) );
		REQUIRE_FALSE( s1.is_global() );
		REQUIRE_FALSE( s1.is_local_to(s1) );

		auto& s2 = s1.push_scope();
		REQUIRE( s2.depth() == 2 );
		REQUIRE( s2.parent() == &s1 );
		REQUIRE( s2.is_local_to(s1) );
		REQUIRE( s2.is_descendent_of(gs) );
		REQUIRE( s2.is_descendent_of(s1) );
		REQUIRE_FALSE( s2.is_global() );
		REQUIRE_FALSE( s2.is_local_to(gs) );
		REQUIRE_FALSE( s2.is_local_to(s2) );
	}
}