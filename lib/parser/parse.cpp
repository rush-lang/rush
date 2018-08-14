#include "rush/core/iterator.hpp"

#include "printer.hpp"
#include "parser.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>


namespace rush {
	const lexical_token parser::eof = tokens::eof();

	parse_result parse(lexical_analysis const& lxa, parser_options const& opts) {
		auto p = parser { opts };
		auto root = p.parse(lxa);

		return root != nullptr
			? parse_result { std::move(root) }
			: parse_result {};
	}

	parse_result parse(std::string input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}

	parse_result parse(std::istream& input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}


	void dump(std::string input, std::ostream& out) {
		dump(parse(input), out);
	}

	void dump(std::istream& input, std::ostream& out) {
		dump(parse(input), out);
	}

	void dump(std::string input, parser_options opts, std::ostream& out) {
		dump(parse(input, opts), out);
	}

	void dump(std::istream& input, parser_options opts, std::ostream& out) {
		dump(parse(input, opts), out);
	}

	void dump(parse_result const& input, std::ostream& out) {
		if (input.ast() != nullptr)
			input.ast()->accept(ast::printer { out });
	}

} // rush