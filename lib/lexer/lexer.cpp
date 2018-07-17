#include "rush/core/charinfo.hpp"
#include "rush/core/iterator.hpp"
#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"

#include <string>
#include <iostream>
#include <iterator>

using namespace rush;
using namespace rush::charinfo;

// namespace rtok = rush::tokens;


namespace rush {

	template <typename InIter>
	void _lex(InIter first, InIter last, lexer_options const& opts);

	lexical_analysis lex(char const* input, lexer_options const& opts) {
		auto last = input + std::strlen(input);
		// auto l = lexer { input, last, opts };
		// return { begin(l), end(l) };
		return {};
	}

	lexical_analysis lex(std::string const& input, lexer_options const& opts) {
		auto first = begin(input);
		auto last = end(input);
		_lex(first, last, opts);
		return {};
	}

	lexical_analysis lex(std::istream& input, lexer_options const& opts) {
		std::istream_iterator<char> it { input >> std::noskipws }, eof;
		_lex(it, eof, opts);
		return {};
	}



	template <typename InIter>
	void lex(InIter first, InIter last, lexer_options const& opts) {
		std::cout << "hello world!" << std::endl;
	}

	template <typename InIter>
	auto scan_integer_literal(InIter& first, InIter const& last) {
		assert(first != last && is_digit(*first) && "expected a leading digit while scanning an integer literal.");
		return advance_if(first, last, is_digit);
	}

	template <typename InIter>
	auto scan_string_literal(InIter& first, InIter const& last) {
		assert(first != last && *first == '"' && "expected a leading double quotation mark while scanning a string literal.");
		return advance_if(first, last, [](auto& ch) { return ch != '"'; });
	}

	template <typename InIter>
	auto scan_identifier(InIter& first, InIter const& last) {
		bool result = false;
		if (result = advance_if(first, last, is_ident_head); result)
			advance_if(first, last, is_ident_body);
		return result;
	}
} // namespace rush
