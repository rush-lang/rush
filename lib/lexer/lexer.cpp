#include "rush/core/charinfo.hpp"
#include "rush/core/iterator.hpp"
#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"

#include <string>
#include <iostream>
#include <iterator>
#include <vector>


using namespace rush;
namespace tok = rush::tokens;
namespace ci = rush::charinfo;


template <typename FwdIter> class lexer;
template <typename FwdIter> static lexer<FwdIter> lex(FwdIter first, FwdIter last, lexer_options const& opts);



template <typename FwdIter>
class lexer {
	friend lexer<FwdIter> lex<FwdIter>(FwdIter first, FwdIter last, lexer_options const& opts);

public:
	lexer(lexer const&) = delete;
	void operator = (lexer const&) = delete;

	lexer(lexer&& other)
		: _opts(other._opts)
		, _tokens(std::move(other._tokens)) {}

	void operator = (lexer&& other) {
		// fixme: check other is not this.
		_opts = other._opts;
		_tokens = std::move(other._tokens);
	}


	void tokenize(FwdIter first, FwdIter last) {

	}

	std::vector<lexical_token> flush() {
		return std::move(_tokens);
	}

private:
	lexer(rush::lexer_options opts) : _opts(opts) {}

	std::vector<lexical_token> _tokens;
	lexer_options _opts;
};



template <typename FwdIter>
static lexer<FwdIter> lex(FwdIter first, FwdIter last, lexer_options const& opts) {
	auto l = lexer<FwdIter> { opts };
	l.tokenize(first, last);
	return std::move(l);
}

template <typename InIter>
static lexical_token make_punctuation_token(InIter first, InIter last) {
	assert(first != last && "unexpected end of range.");
	assert(is_punct(*first) && "expected punctuation.");

}

template <typename InIter>
static lexical_token make_compounded_punctuation_token(InIter first, InIter last, char const* comp_chars) {
	assert(first != last && "unexpected end of range.");
	assert(is_punct(*first) && "expected punctuation.");

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