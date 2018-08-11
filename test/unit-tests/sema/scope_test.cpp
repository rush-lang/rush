#include "catch2/catch.hpp"
#include "rush/sema/scope.hpp"

TEST_CASE( "rush::scope", "[unit][sema]" ) {
	REQUIRE( rush::global_scope.depth() == 0 );
	REQUIRE( rush::global_scope.parent() == nullptr );

	auto& ls = rush::define_scope(rush::global_scope);
	REQUIRE( ls.depth() == 1 );
	REQUIRE( ls.parent() == &rush::global_scope );
}