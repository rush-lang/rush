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
			skip_whitespace();
			_tokens.push_back(next_token());
		}
	}

	std::vector<lexical_token> flush() {
		return std::move(_tokens);
	}

private:
	lexer_options _options;
	location _pinloc;
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

	location location() noexcept {
		return _pinloc;
	}

	void pin_location() noexcept {
		_pinloc = { _iters.first.location() };
	}

	codepoint_t peek(std::size_t offset = 0) {
		assert(!eof() && "unexpected end of source.");
		auto temp = _iters.first;
		if (advance(temp, _iters.second, offset))
			return temp != _iters.second ? *temp : npos_codepoint;
		return *temp;
	}

	bool check(codepoint_t cp, std::size_t offset = 0) {
		return !eof() && peek(offset) == cp;
	}

	template <typename Pred>
	auto check(Pred predicate, std::size_t offset = 0) -> decltype(predicate(codepoint_t{}), bool{}) {
		return !eof() && predicate(peek(offset));
	}

	void skip(std::size_t offset = 1) {
		advance(_iters.first, _iters.second, offset);
	}

	void skip_whitespace() {
		advance_if(_iters.first, _iters.second, is_space);
	}

	lexical_token next_token() {
		auto cp = peek();

		pin_location();

		if (is_quote(cp)) return scan_string_literal();
		if (is_digit(cp)) return scan_numeric_literal();
		if (is_ident_head(cp)) return scan_identifier();

		auto str = std::string { static_cast<char>(cp) };
		auto symbol = symbols::to_value(str);
		if (symbol != symbols::unknown) {
			skip(); // consume symbol character.
			return tok::make_symbol_token(symbol);
		}

		// return tok::make_error_token("unexpected token", location());
		return tok::eof(location());
	}

	template <typename Pred>
	auto scan_while(Pred predicate) -> decltype(predicate(*_iters.first), std::string()) {
		auto temp = _iters.first;
		advance_if(_iters.first, _iters.second, predicate);
		return std::string(temp, _iters.first);
	}

	lexical_token scan_string_literal() {
		skip();
		return tok::string_literal("", location());
	}

	lexical_token scan_numeric_literal() {
		assert(!eof() && "unexpected end of source.");
		assert(is_digit(peek()) && "expected a leading digit while attempting to scan an integer literal.");

		if (is_zero_digit(peek())) {
			skip(); // consume zero digit.
			return check('.')
				? scan_floating_literal()
				: tok::integer_literal(0, location());
		}

		auto value = scan_while(is_digit);
		return check('.')
			? scan_floating_literal(value + ".")
			: tok::integer_literal(std::stoll(value), location());
	}

	lexical_token scan_floating_literal(std::string prefix = "") {
		assert(!eof() && "unexpected end of source.");
		assert(peek() == '.' && "expected a leading decimal point while attempting to scan a floating literal.");

		skip(); // consume decimal point.
		auto value = scan_while(is_digit);
		return tok::floating_literal(std::stod(prefix + value), location());
	}

	lexical_token scan_identifier() {
		assert(!eof() && "unexpected end of source.");
		assert(is_ident_head(peek()) && "expected a leading identifier character while attempting to scan an identifier.");

		auto ident = scan_while(is_ident_body);
		auto kw_val = keywords::to_value(ident);
		return kw_val != keywords::unknown
			? tok::make_keyword_token(kw_val, location())
			: tok::identifier(ident, location());
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