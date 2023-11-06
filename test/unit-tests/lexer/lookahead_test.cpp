/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#include <catch2/catch_all.hpp>
#include "rush/lexer/lookahead.hpp"

#include <vector>

TEST_CASE( "rush::lookahead_buffer", "[unit][lexer]" ) {

	GIVEN( "A vector of 'n' elements" ) {
		std::vector<int> v { 1, 2, 3 };

		AND_THEN( "A lookahead_buffer of 'k' < 'n'" ) {
			auto lab = rush::lookahead<2>(v);

			THEN( "The buffer should not be empty()" ) { REQUIRE_FALSE( lab.empty() ); }
			THEN( "The buffers size() should be at capacity()" ) { REQUIRE( lab.size() == 2 ); }
			THEN( "The buffers capacity() should be == 'k'" ) { REQUIRE( lab.capacity() == 2 ); }

			THEN( "peek()'ing at the first element of the buffer" ) { REQUIRE( lab.peek() == 1 ); }
			THEN( "peek()'ing at the last element of the buffer" ) { REQUIRE( lab.peek(1) == 2 ); }
			THEN( "peek()'ing past the last element of the buffer" ) { /* should throw */ }

			WHEN( "A single element is consumed with next()" ) {
				auto prev = lab.next();

				THEN( "The buffer should not be empty()" ) { REQUIRE_FALSE( lab.empty() ); }
				THEN( "The buffers size() should be at capacity" ) { REQUIRE( lab.size() == 2 ); }
				THEN( "The buffers capacity() should be == 'k'" ) { REQUIRE( lab.capacity() == 2 ); }

				THEN( "peek()'ing at the first element" ) { REQUIRE( lab.peek() == 2 ); }
				THEN( "peek()'ing at the last element" ) { REQUIRE( lab.peek(1) == 3 ); }
				THEN( "The element returned should be the one popped from the front of the buffer" ) { REQUIRE( prev == 1 ); }
			}

			WHEN( "All elements in the buffer are consumed with skip()" ) {
				lab.skip(v.size());

				THEN( "The buffer should be empty()" ) { REQUIRE( lab.empty() ); }
				THEN( "The buffer size() should be == 0" ) { REQUIRE( lab.size() == 0 ); }
				THEN( "The buffer capacity() should still be == 'k'" ) { REQUIRE( lab.capacity() == 2 ); }
			}
		}

		AND_THEN( "A lookahead_buffer of 'k' > 'n'" ) {
			auto lab = rush::lookahead<4>(v);

			THEN( "The buffer should not be empty()" ) { REQUIRE_FALSE( lab.empty() ); }
			THEN( "The buffers size() should be == 'n'" ) { REQUIRE( lab.size() == 3 ); }
			THEN( "The buffers capacity() should be == 'k'" ) { REQUIRE( lab.capacity() == 4 ); }

			THEN( "peek()'ing at the first element of the buffer" ) { REQUIRE( lab.peek() == 1 ); }
			THEN( "peek()'ing at the last element of the buffer" ) { REQUIRE( lab.peek(2) == 3 ); }
			THEN( "peek()'ing past the last element of the buffer" ) { /* should throw */ }

			WHEN( "A single element is consumed with next()" ) {
				auto prev = lab.next();

				THEN( "The buffer should not be empty()" ) { REQUIRE_FALSE( lab.empty() ); }
				THEN( "The buffers size() should be reduced" ) { REQUIRE( lab.size() == 2 ); }
				THEN( "The buffers capacity() should be == 'k'" ) { REQUIRE( lab.capacity() == 4 ); }

				THEN( "peek()'ing at the first element" ) { REQUIRE( lab.peek() == 2 ); }
				THEN( "peek()'ing at the last element" ) { REQUIRE( lab.peek(1) == 3 ); }
				THEN( "The element returned should be the one popped from the front of the buffer" ) { REQUIRE( prev == 1 ); }
			}

			WHEN( "All elements in the buffer are consumed with skip()" ) {
				lab.skip(v.size());

				THEN( "The buffer should be empty()" ) { REQUIRE( lab.empty() ); }
				THEN( "The buffer size() should be == 0" ) { REQUIRE( lab.size() == 0 ); }
				THEN( "The buffer capacity() should still be == 'k'" ) { REQUIRE( lab.capacity() == 4 ); }
			}
		}
	}
}
