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

	class lexer_iterator {

	private:
		std::size_t _currentIndent;
	};

	class lexer {
	public:
		using iterator = lexer_iterator;
		using const_iterator = lexer_iterator;

		auto peek() -> lexical_token;
		auto next() -> lexical_token;



	private:
		std::size_t _currentIndent;

	};


	template <typename InIter>
	auto lex(InIter first, InIter last, lexer_options const& opts) -> void {
		std::cout << "hello world!" << std::endl;
	}

	auto lex(std::string const& input, lexer_options const& opts) -> lexical_analysis {
		auto first = begin(input);
		auto last = end(input);
		lex(first, last, opts);
		return {};
	}

	auto lex(std::istream& input, lexer_options const& opts) -> lexical_analysis {
		std::istream_iterator<char> it(input >> std::noskipws), eof;
		lex(it, eof, opts);
		return {};
	}


	template <typename InIter>
	auto scan_integer(InIter& first, InIter const& last) {
		return advance_if(first, last, is_digit);
	}

	template <typename InIter>
	auto scan_identifier(InIter& first, InIter const& last) {
		bool result = false;
		if (result = advance_if(first, last, is_ident_head); result)
			advance_if(first, last, is_ident_body);
		return result;
	}
} // namespace rush
