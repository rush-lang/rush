#include "catch2/catch.hpp"
#include "rush/core/ring.hpp"

#include <algorithm>

template <typename T, std::size_t N>
bool equal(rush::ring<T, N> const& rng, std::initializer_list<T> init) {
	return std::equal(rng.begin(), rng.end(), init.begin(), init.end());
}

TEST_CASE( "rush::ring", "[unit][core]" ) {
	GIVEN( "an empty ring of capacity == 0" ) {
		rush::ring<int, 0> r;
		THEN( "capacity() should == 0" ) { REQUIRE( r.capacity() == 0 ); }
	}

	GIVEN( "an empty ring of capacity > 0" ) {
		rush::ring<int, 3> r;

		THEN( "empty() should return true" ) { REQUIRE( r.empty() ); }
		THEN( "size() should be zero" ) { REQUIRE( r.size() == 0 ); }
		THEN( "capacity() should be set" ) { REQUIRE( r.capacity() == 3 ); }

		WHEN ( "pushing elements to the back" ) {
			r.push_back(1);

			THEN( "empty() should return false" ) { REQUIRE_FALSE( r.empty() ); }
			THEN( "size() should be incremented" ) { REQUIRE( r.size() == 1 ); }
			THEN( "front() should be equal to back()") { REQUIRE( &r.front() == &r.back() ); }

			AND_THEN( "filling the ring to capacity" ) {
				r.push_back(2);
				r.push_back(3);

				THEN( "front should be the first value inserted" ) { REQUIRE( r.front() == 1 ); }
				THEN( "back should be the last value inserted" ) { REQUIRE( r.back() == 3 ); }
				THEN( "size() should be incremented" ) { REQUIRE( r.size() == 3 ); }
				THEN( "capacity() should == size()" ) { REQUIRE( r.capacity() == r.size() ); }

				AND_WHEN( "iterating over the ring" ) {
					THEN( "elements should appear in FIFO order" ) { REQUIRE( equal(r, { 1, 2, 3 }) ); }
				}

				AND_WHEN( "pushing another element" ) {
					r.push_back(4);

					THEN( "the first element should be dropped" ) {
						REQUIRE( r.front() == 2 );
						REQUIRE( *r.begin() == 2 );
						REQUIRE( &r.front() == &(*r.begin()) );

						AND_THEN( "the new element is pushed to the back of the ring" ) {
							REQUIRE( r.back() == 4 );
							// REQUIRE( *(r.end() - 1) == 4 );
							// REQUIRE( &r.back() == &(*(r.end() - 1)) );
						}

						AND_WHEN( "iterator over the ring") {
							THEN( "elements should be as expected" ) { REQUIRE( equal(r, { 2, 3, 4 }) ); }
						}
					}
				}
			}
		}

		WHEN( "pushing elements to the front" ) {
			r.push_front(1);

			THEN( "empty() should return false" ) { REQUIRE_FALSE( r.empty() ); }
			THEN( "size() should be incremented" ) { REQUIRE( r.size() == 1 ); }
			THEN( "front() should be equal to back()") { REQUIRE( &r.front() == &r.back() ); }

			AND_THEN( "filling the ring to capacity" ) {
				r.push_front(2);
				r.push_front(3);

				THEN( "front should be the last value inserted" ) { REQUIRE( r.front() == 3 ); }
				THEN( "back should be the first value inserted" ) { REQUIRE( r.back() == 1 ); }
				THEN( "size() should be incremented" ) { REQUIRE( r.size() == 3 ); }
				THEN( "capacity() should == size()" ) { REQUIRE( r.capacity() == r.size() ); }

				AND_WHEN( "iterating over the ring" ) {
					THEN( "elements should appear in LIFO order" ) { REQUIRE( equal(r, { 3, 2, 1 }) ); }
				}

				AND_WHEN( "pushing another element" ) {
					r.push_front(4);

					THEN( "the last element should be dropped" ) {
						REQUIRE( r.back() == 2 );
						// REQUIRE( *(r.end() - 1) == 2 );
						// REQUIRE( &r.back() == &(*r.end()) );

						AND_THEN( "the new element is pushed to the back of the ring" ) {
							REQUIRE( r.front() == 4 );
							REQUIRE( *r.begin() == 4 );
							REQUIRE( &r.front() == &(*r.begin()) );
						}

						AND_WHEN( "iterator over the ring") {
							THEN( "elements should be as expected" ) { REQUIRE( equal(r, { 4, 3, 2 }) ); }
						}
					}
				}
			}
		}
	}
}