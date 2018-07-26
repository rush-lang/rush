#include "rush/core/charinfo.hpp"
#include "rush/core/iterator.hpp"
#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"
#include "rush/lexer/indentation.hpp"

#include "lexer_iterator.hpp"

#include <string>
#include <iostream>
#include <iterator>
#include <vector>


using namespace rush;
using namespace rush::charinfo;

namespace tok = rush::tokens;

template <typename FwdIter> class lexer;
template <typename FwdIter> static lexer<FwdIter> lex(FwdIter first, FwdIter last, lexer_options const& opts);


template <typename FwdIter>
class lexer final {
	friend lexer<FwdIter> lex<FwdIter>(FwdIter, FwdIter, lexer_options const&);

public:
	lexer(lexer const&) = delete;
	void operator = (lexer const&) = delete;

	lexer(lexer&& other)
		: _options {other._options }
		, _indentation { }
		, _tokens(std::move(other._tokens)) {}

	void operator = (lexer&& other) {
		// fixme: check other is not this.
		_options = other._options;
		_indentation = other._indentation;
		_tokens = std::move(other._tokens);
	}


	void tokenize(FwdIter first, FwdIter last) {
		_iters = {
			lexer_iterator { first, last, { } },
			lexer_iterator { last, last, location::undefined }
		};

		while (!this->eof()) {
			_tokens.push_back(next_token());
		}
	}

	std::vector<lexical_token> flush() {
		return std::move(_tokens);
	}

private:
	lexer_options _options;
	indentation _indentation;
	std::vector<lexical_token> _tokens;
	std::pair<
		lexer_iterator<FwdIter>,
		lexer_iterator<FwdIter>> _iters;


	lexer(rush::lexer_options opts)
		: _options(opts) {}

	inline bool eof() {
		return _iters.first == _iters.second;
	}

	location location() const noexcept {
		return _iters.first.location();
	}

	codepoint_t peek() {
		assert(!this->eof() && "unexpected end of source.");
		return *_iters.first;
	}


	lexical_token next_token() {
		auto cp = peek();
		if (is_digit(cp)) {
			return scan_numeric_literal();
		}

		if (is_ident_head(cp)) {
			return scan_identifier();
		}

		return tok::eof();
	}

	lexical_token scan_numeric_literal() {
		assert(!eof() && "unexpected end of source.");
		assert(is_digit(peek()) && "expected a leading digit while attempting to scan an integer literal.");

		if (is_zero_digit(*_iters.first)) {
			return tok::integer_literal(0, location());
		}

		auto temp = _iters.first;
		advance_if(_iters.first, _iters.second, is_digit);
		return tok::integer_literal(
			std::stoll(std::string(temp, _iters.first)),
			location());
	}

	lexical_token scan_identifier() {
		return tok::identifier("x", location());
	}
};



template <typename FwdIter>
static lexer<FwdIter> lex(FwdIter first, FwdIter last, lexer_options const& opts) {
	auto l = lexer<FwdIter> { opts };
	l.tokenize(first, last);
	return std::move(l);
}


namespace rush {
	lexical_analysis lex(char const* input, lexer_options const& opts) {
		auto first = input;
		auto last = input + std::strlen(input);
		auto l = ::lex(first, last, opts);
		return { l.flush() };
	}

	lexical_analysis lex(std::string const& input, lexer_options const& opts) {
		auto first = begin(input);
		auto last = end(input);
		auto l = ::lex(first, last, opts);
		return { l.flush() };
	}

	// FIXME: Wrap istreambuf_iterator in some kind of 'buffered_iterator'
	// promoting it to the required forward iterator type.
	lexical_analysis lex(std::istream& input, lexer_options const& opts) {
		auto it = std::istreambuf_iterator<char> { input };
		auto eof = std::istreambuf_iterator<char> {};
		auto l = ::lex(it, eof, opts);
		return { l.flush() };
	}
} // namespace rush