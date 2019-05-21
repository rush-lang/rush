// #include "catch2/catch.hpp"
// #include "rush/rush/attributes.hpp"

// TEST_CASE( "rush::symbol_kind", "[unit][parser]" ) {

// 	auto flags = rush::make_attributes(rush::symbol_kind::undefined);
// 	REQUIRE( rush::has_attributes(flags, rush::symbol_kind::undefined) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::type) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::function) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::constant) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::variable) );

// 	flags = rush::make_attributes(rush::symbol_kind::type);
// 	REQUIRE( rush::has_attributes(flags, rush::symbol_kind::type) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::undefined) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::function) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::constant) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::symbol_kind::variable) );
// }

// TEST_CASE( "rush::access_modifier", "[unit][parser]" ) {

// 	auto flags = rush::make_attributes(rush::access_modifier::internal);
// 	REQUIRE( rush::has_attributes(flags, rush::access_modifier::internal) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::exported) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::protected_) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::private_) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::public_) );

// 	flags = rush::make_attributes(rush::access_modifier::exported);
// 	REQUIRE( rush::has_attributes(flags, rush::access_modifier::exported) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::internal) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::protected_) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::private_) );
// 	CHECK_FALSE( rush::has_attributes(flags, rush::access_modifier::public_) );
// }
