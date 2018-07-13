#include "rush/parser/parser.hpp"
#include <sstream>

namespace rush {

	abstract_syntax_tree parse(std::string input, parser_options const& opts) {
		auto iss = std::istringstream(input);
		return parse(iss, opts);
	}

	abstract_syntax_tree parse(std::istream& input, parser_options const& opts) {
		return {};
	}
} // rush