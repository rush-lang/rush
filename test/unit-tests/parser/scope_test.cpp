#include "catch2/catch.hpp"
#include "rush/parser/scope.hpp"



TEST_CASE( "rush::scope", "[unit][parser]" ) {

	// GIVEN( "An instance of scope, 's1'" ) {
	// 	auto s1 = rush::scope {};

	// 	THEN( "is_global() should be true" ) { REQUIRE( s1.is_global() ); }
	// 	THEN( "is_local() should be false" ) { REQUIRE_FALSE( s1.is_local() ); }
	// 	THEN( "parent() should be empty" ) { REQUIRE( s1.parent() == nullptr ); }

	// 	AND_THEN( "Another instance of scope, 's2'" ) {
	// 		auto s2 = rush::scope {};

	// 		WHEN( "attach()'ing the second instance of scope to the first" ) {
	// 			s1.attach(s2);

	// 			THEN( "is_global() of 's1' should remain true" ) { REQUIRE( s1.is_global() ); }
	// 			THEN( "is_local() of 's1' should remain false" ) { REQUIRE_FALSE( s1.is_local() ); }
	// 			THEN( "parent() of 's1' should remain empty" ) { REQUIRE( s1.parent() == nullptr ); }

	// 			THEN( "is_local() of 's2' should be true" ) { REQUIRE( s2.is_local() ); }
	// 			THEN( "is_global() of 's2' should be false" ) { REQUIRE_FALSE( s2.is_global() ); }
	// 			THEN( "parent() of 's2' should be 's1'" ) { REQUIRE( s2.parent() == std::addressof(s1) ); }

	// 			AND_WHEN( "detach()'ing the second instance of scope" ) {
	// 				s2.detach();

	// 				THEN( "is_global() of 's2' should be true" ) { REQUIRE( s2.is_global() ); }
	// 				THEN( "is_local() of 's2' should be false" ) { REQUIRE_FALSE( s2.is_local() ); }
	// 				THEN( "The parent() of 's2' should be empty" ) { REQUIRE( s2.parent() == nullptr ); }
	// 			}
	// 		}
	// 	}
	// }

	// SECTION( "nesting scopes" ) {

	// 	auto& gs = rush::global_scope;
	// 	REQUIRE( gs.depth() == 0 );
	// 	REQUIRE( gs.parent() == nullptr );
	// 	REQUIRE( gs.is_global() );
	// 	REQUIRE_FALSE( gs.is_local_to(gs) );

	// 	auto& s1 = gs.push_scope();
	// 	REQUIRE( s1.depth() == 1 );
	// 	REQUIRE( s1.parent() == &gs );
	// 	REQUIRE( s1.is_local_to(gs) );
	// 	REQUIRE_FALSE( s1.is_global() );
	// 	REQUIRE_FALSE( s1.is_local_to(s1) );

	// 	auto& s2 = s1.push_scope();
	// 	REQUIRE( s2.depth() == 2 );
	// 	REQUIRE( s2.parent() == &s1 );
	// 	REQUIRE( s2.is_local_to(s1) );
	// 	REQUIRE( s2.is_descendent_of(gs) );
	// 	REQUIRE( s2.is_descendent_of(s1) );
	// 	REQUIRE_FALSE( s2.is_global() );
	// 	REQUIRE_FALSE( s2.is_local_to(gs) );
	// 	REQUIRE_FALSE( s2.is_local_to(s2) );
	// }
}
