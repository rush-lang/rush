#include "catch2/catch.hpp"
#include "rush/sema/scope.hpp"

TEST_CASE( "rush::scope", "[unit][sema]" ) {

	SECTION( "nesting scopes" ) {

		auto& gs = rush::global_scope;
		REQUIRE( gs.depth() == 0 );
		REQUIRE( gs.parent() == nullptr );
		REQUIRE( gs.is_global() );
		REQUIRE_FALSE( gs.is_local_to(gs) );

		auto& ls1 = gs.push_block_scope();
		REQUIRE( ls1.depth() == 1 );
		REQUIRE( ls1.parent() == &gs );
		REQUIRE( ls1.is_local_to(gs) );
		REQUIRE_FALSE( ls1.is_global() );
		REQUIRE_FALSE( ls1.is_local_to(ls1) );

		auto& ls2 = ls1.push_block_scope();
		REQUIRE( ls2.depth() == 2 );
		REQUIRE( ls2.parent() == &ls1 );
		REQUIRE( ls2.is_local_to(ls1) );
		REQUIRE( ls2.is_descendent_of(gs) );
		REQUIRE( ls2.is_descendent_of(ls1) );
		REQUIRE_FALSE( ls2.is_global() );
		REQUIRE_FALSE( ls2.is_local_to(gs) );
		REQUIRE_FALSE( ls2.is_local_to(ls2) );
	}
}