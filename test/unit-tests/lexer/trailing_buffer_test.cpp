#include "catch2/catch.hpp"
#include "rush/lexer/trailing_buffer.hpp"

#include <vector>
#include <sstream>

template <typename InIter, std::size_t N>
bool equal(rush::trailing_buffer<InIter, N>& buf, std::initializer_list<int> init) {
	return std::equal(init.begin(), init.end(), buf.begin());
}

TEST_CASE( "rush::trailing_buffer" ) {

	GIVEN( "a vector of 'n' elements" ) {
		std::vector<int> v { 1, 2, 3, 4 };
		AND_THEN( "a trailing buffer of fixed size 'k', less than the total number of elements in the vector" ) {
			auto buf = rush::make_trailing_buffer<3>(v.begin(), v.end());

			THEN( "the buffer should be initialized with the first 'k' elements of the vector" ) {
				REQUIRE( equal(buf, { 1, 2, 3 }) );
			}

			WHEN("iterating past the fixed size 'k' to 'n'" ) {

				THEN( "the buffer should output the elements in the order they were buffered.") {
					equal(buf, { 1, 2, 3, 4 });

					AND_THEN("subsequent iterations over the buffer should only output the last 'k' elements buffered") {
						equal(buf, { 2, 3, 4 });
						equal(buf, { 2, 3, 4 });
					}
				}
			}
		}
	}

	GIVEN( "an input stream of 'n' elements" ) {
		std::istringstream is { "1 2 3 4" };
		AND_THEN( "a trailing buffer of fixed size 'k', less than the total number of elements in the input stream" ) {
			auto buf = rush::make_trailing_buffer<3>(
				std::istream_iterator<int> { is },
				std::istream_iterator<int> { });

			THEN( "the buffer should be initialized with the first 'k' elements of the input stream" ) {
				REQUIRE( equal(buf, { 1, 2, 3 }) );
			}

			WHEN("iterating past the fixed size 'k' to 'n'" ) {

				THEN( "the buffer should output the elements in the order they were buffered.") {
					equal(buf, { 1, 2, 3, 4 });

					AND_THEN("subsequent iterations over the buffer should only output the last 'k' elements buffered") {
						equal(buf, { 2, 3, 4 });
						equal(buf, { 2, 3, 4 });
					}
				}
			}
		}
	}
}