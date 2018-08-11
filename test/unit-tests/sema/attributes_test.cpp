#include "catch2/catch.hpp"
#include "rush/sema/attributes.hpp"

namespace sema = rush::sema;

TEST_CASE( "rush::sema::symbol_type", "[unit][sema]" ) {

	auto flags = sema::make_flags(sema::symbol_type::undefined);
	REQUIRE( sema::has_flags(flags, sema::symbol_type::undefined) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::type) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::function) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::constant) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::variable) );

	flags = sema::make_flags(sema::symbol_type::type);
	REQUIRE( sema::has_flags(flags, sema::symbol_type::type) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::undefined) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::function) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::constant) );
	CHECK_FALSE( sema::has_flags(flags, sema::symbol_type::variable) );
}

TEST_CASE( "rush::sema::access_modifier", "[unit][sema]" ) {

	auto flags = sema::make_flags(sema::access_modifier::internal);
	REQUIRE( sema::has_flags(flags, sema::access_modifier::internal) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::exported) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::protected_) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::private_) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::public_) );

	flags = sema::make_flags(sema::access_modifier::exported);
	REQUIRE( sema::has_flags(flags, sema::access_modifier::exported) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::internal) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::protected_) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::private_) );
	CHECK_FALSE( sema::has_flags(flags, sema::access_modifier::public_) );
}