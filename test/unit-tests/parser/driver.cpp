#include <catch2/catch_all.hpp>
#include "generator.hpp"

TEST_CASE("rush::parser::expressions", "[unit][parser]") {
   auto test = GENERATE( fixture("fixtures/expressions.yaml") );
   SECTION( test.name ) {
      REQUIRE_THAT( test.actual, Catch::Matchers::Equals(test.expected) );
   }
}
