#include "catch2/catch.hpp"
#include "rush/core/ring.hpp"
#include "rush/core/iterator.hpp"

#include <algorithm>
#include <iostream>

template <typename T, std::size_t N>
bool equal(rush::ring<T, N> const& rng, std::initializer_list<T> init) {
	// std::copy(rng.begin(), rng.end(), std::ostream_iterator<T> { std::cout });
	return std::equal(rng.begin(), rng.end(), init.begin(), init.end());
}


template <typename T, std::size_t N>
bool requal(rush::ring<T, N> const& rng, std::initializer_list<T> init) {
	// std::copy(rng.rbegin(), rng.rend(), std::ostream_iterator<T> { std::cout });
	return std::equal(rng.rbegin(), rng.rend(), init.begin(), init.end());
}

TEST_CASE( "rush::ring", "[unit][core]" ) {
	GIVEN( "an empty ring of capacity == 0" ) {
		rush::ring<int, 0> r { };
		THEN( "capacity() should == 0" ) { REQUIRE( r.capacity() == 0 ); }
	}

	GIVEN( "an empty ring of capacity 'n'" ) {
		rush::ring<int, 3> r { };

		THEN( "empty() should return true" ) { REQUIRE( r.empty() ); }
		THEN( "size() should be zero" ) { REQUIRE( r.size() == 0 ); }
		THEN( "capacity() should be set" ) { REQUIRE( r.capacity() == 3 ); }


		WHEN ( "pushing elements to the back" ) {
			r.push_back(1);

			THEN( "empty() should return false" ) { REQUIRE_FALSE( r.empty() ); }
			THEN( "size() should be incremented" ) { REQUIRE( r.size() == 1 ); }
			THEN( "front() should be equal to back()" ) { REQUIRE( &r.front() == &r.back() ); }
			THEN( "begin() should point to the element" ) { REQUIRE( *r.begin() == 1 ); }

			AND_THEN( "filling the ring to capacity" ) {
				r.push_back(2);
				r.push_back(3);

				THEN( "front should be the first value inserted" ) { REQUIRE( r.front() == 1 ); }
				THEN( "back should be the last value inserted" ) { REQUIRE( r.back() == 3 ); }
				THEN( "size() should be incremented" ) { REQUIRE( r.size() == 3 ); }
				THEN( "size() shoudl be equal to capacity()" ) { REQUIRE( r.capacity() == r.size() ); }

				AND_THEN( "iterating over the ring" ) {
					THEN( "elements should appear in FIFO order" ) { REQUIRE( equal(r, { 1, 2, 3 }) ); }
				}

				// AND_THEN( "pushing n - 1 elements" ) {
				// 	auto it = --r.end();
				// 	r.push_back(4);
				// 	r.push_back(5);

				// 	THEN( "iterators should not be invalidated" ) { REQUIRE( *it == 3 ); }
				// }

				// AND_THEN( "pushing n additional elements" ) {
				// 	auto it = --r.end();
				// 	r.push_back(4);
				// 	r.push_back(5);
				// 	r.push_back(6);

				// 	THEN( "iterators should be invalidated" ) { REQUIRE( *it != 3 ); }
				// }

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

						AND_WHEN( "iterating over the ring") {
							THEN( "elements should be as expected" ) { REQUIRE( equal(r, { 2, 3, 4 }) ); }
							THEN( "elements should be as expected (reverse)" ) { REQUIRE( requal(r, { 4, 3, 2 }) ); }
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

TEST_CASE( "rush::ring::iterator", "[unit][core]" ) {
	GIVEN( "A ring of 'n' elements" ) {
		rush::ring<int, 3> r { 1, 2, 3 };

		AND_THEN( "An iterator to the end of the ring" ) {
			auto it = r.end();

			THEN( "The iterator should be the same as end()" ) { REQUIRE( it == r.end() ); }
			THEN( "The iterator should not be the same as begin()" ) { REQUIRE( it != r.begin() ); }
			THEN( "The iterator should point to 1 past the last element" ) {
				--it;
				REQUIRE( *it == 3 );
				REQUIRE( &(*it) == &r.back() );
			}

			// WHEN( "incrementing the iterator once" ) {
			// 	++it;
			// 	THEN( "the iterator should circle back and point to the first element" ) {
			// 		REQUIRE( *it == 1 );
			// 		REQUIRE( &(*it) == &r.front() );
			// 	}
			// }
		}

		AND_THEN("An iterator to the beginning of the ring" ) {
			auto it = r.begin();

			THEN( "The iterator should be the same as begin()" ) { REQUIRE( it == r.begin() ); }
			THEN( "The iterator should not be the same as end()" ) { REQUIRE( it != r.end() ); }
			THEN( "The iterator should point to first element in the ring" ) {
				REQUIRE( *it == 1 );
				REQUIRE( &(*it) == &r.front() );
			}

			WHEN( "incrementing the iterator once" ) {
				rush::advance(it, r.end(), 1);
				THEN( "the iterator should point to the second element in the ring" ) {
					REQUIRE( *it == 2 );
				}
			}

			WHEN( "incrementing the iterator to the last element" ) {
				rush::advance(it, r.end(), 2);
				THEN( "the iterator should point to the last element in the ring" ) {
					REQUIRE( *it == 3 );
					REQUIRE( &(*it) == &r.back() );
				}
			}

			WHEN( "incrementing the iterator past the last element" ) {
				++it; ++it; ++it;
				// rush::advance(it, ++r.end());
				THEN( "the iterator should be equal to end()" ) {
					REQUIRE( it == r.end() );
				}
			}
		}
	}
}