#include "rush/lexer/analysis.hpp"
#include "rush/lexer/dump.hpp"
#include "rush/lexer/lex.hpp"

#include <iostream>

namespace rush {
	void dump(lexical_analysis const& input) {
		return dump(input, std::cout);
	}

	void dump(lexical_analysis const& input, std::ostream& out) {
		for (auto& tok : input) out
			<< rush::debug_string(tok)
			<< std::endl;
	}
} // rush