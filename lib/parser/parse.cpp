#include "rush/lexer/lex.hpp"
#include "rush/parser/options.hpp"
#include "rush/parser/parser.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace rush {
	const lexical_token parser::eof = tokens::eof();

	parse_result parse(lexical_analysis const& lxa, parser_options const& opts) {
		auto p = parser { opts };
		auto root = p.parse(lxa);

		return root != nullptr
			? parse_result { std::move(root) }
			: parse_result {};
	}

	parse_result parse(std::string input) { return parse(input, {}); }
	parse_result parse(std::istream& input) { return parse(input, {}); }

	parse_result parse(std::string input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}

	parse_result parse(std::istream& input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}
} // rush