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

} // namespace rush
