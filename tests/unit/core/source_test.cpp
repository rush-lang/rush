#include "catch2/catch.hpp"
#include "rush/core/source.hpp"

#include <sstream>

TEST_CASE( "rush::source", "[unit][core]" ) {
	auto str = std::stringstream { "main() => 0" };
	auto src = rush::source { str };

}
