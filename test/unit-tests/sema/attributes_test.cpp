#include "catch2/catch.hpp"
#include "rush/sema/attributes.hpp"

namespace sema = rush::sema;

TEST_CASE( "rush::sema::symbol_kind", "[unit][sema]" ) {

	auto flags = sema::make_attributes(sema::symbol_kind::undefined);
	REQUIRE( sema::has_attributes(flags, sema::symbol_kind::undefined) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::type) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::function) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::constant) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::variable) );

	flags = sema::make_attributes(sema::symbol_kind::type);
	REQUIRE( sema::has_attributes(flags, sema::symbol_kind::type) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::undefined) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::function) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::constant) );
	CHECK_FALSE( sema::has_attributes(flags, sema::symbol_kind::variable) );
}

TEST_CASE( "rush::sema::access_modifier", "[unit][sema]" ) {

	auto flags = sema::make_attributes(sema::access_modifier::internal);
	REQUIRE( sema::has_attributes(flags, sema::access_modifier::internal) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::exported) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::protected_) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::private_) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::public_) );

	flags = sema::make_attributes(sema::access_modifier::exported);
	REQUIRE( sema::has_attributes(flags, sema::access_modifier::exported) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::internal) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::protected_) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::private_) );
	CHECK_FALSE( sema::has_attributes(flags, sema::access_modifier::public_) );
}