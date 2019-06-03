#include "rush/lexer/lex.hpp"
#include "rush/parser/options.hpp"
#include "rush/parser/parser.hpp"
#include "rush/parser/parse.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace rush {
	const lexical_token parser::eof = tokens::eof();

	syntax_analysis parse(lexical_analysis const& lxa, parser_options const& opts) {
		auto p = parser { opts };
		auto root = p.parse(lxa);
      return { std::move(root) };
	}

	syntax_analysis parse(std::string input) { return parse(input, {}); }
	syntax_analysis parse(std::istream& input) { return parse(input, {}); }

	syntax_analysis parse(std::string input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}

	syntax_analysis parse(std::istream& input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}
} // rush
