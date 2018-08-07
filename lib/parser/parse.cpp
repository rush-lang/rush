#include "rush/core/iterator.hpp"
#include "parser.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>


namespace rush {
	const lexical_token parser::eof = tokens::eof();

	abstract_syntax_tree parse(lexical_analysis const& lxa, parser_options const& opts) {
		auto p = parser { opts };
		auto root = p.parse(lxa);

		return root != nullptr
			? abstract_syntax_tree { std::move(root) }
			: abstract_syntax_tree {};
	}

	abstract_syntax_tree parse(std::string input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}

	abstract_syntax_tree parse(std::istream& input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}
} // rush