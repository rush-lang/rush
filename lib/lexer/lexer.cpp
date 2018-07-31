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
#include <optional>


using namespace rush;
using namespace rush::charinfo;

namespace tok = rush::tokens;

namespace rush{
	template <typename FwdIter>
	lexical_analysis lex(FwdIter, FwdIter, lexer_options const& opts);
}

template <typename FwdIter>
class lexer final {
	friend lexical_analysis rush::lex<FwdIter>(
		FwdIter, FwdIter,
		lexer_options const&);

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
		if (first == last) return;

		_iters = {
			lexer_iterator { first, last, {} },
			lexer_iterator { last, last, location::undefined }
		};

		auto token = next_token();
		for (; !token.is(symbols::eof); token = next_token()) {
			_tokens.push_back(std::move(token));
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

	bool is_line_start() noexcept {
		return _iters.first.location().column() == 1;
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

	bool check(std::string str, std::size_t offset = 0) {
		for (std::size_t i = 0, j = offset; i < str.length(); ++i, ++j)
			if (!check(str[i], j)) return false;
		return true;
	}

	template <typename Pred>
	auto check(Pred predicate, std::size_t offset = 0)
		-> decltype(predicate(codepoint_t{}), bool{}) {
		return !eof() && predicate(peek(offset));
	}

	void skip(std::size_t offset = 1) {
		advance(_iters.first, _iters.second, offset);
	}

	template <typename Pred>
	void skip_while(Pred predicate) {
		advance_if(_iters.first, _iters.second, predicate);
	}

	template <typename Pred>
	auto scan_while(Pred predicate)
		-> decltype(predicate(*_iters.first), std::string()) {
		auto temp = _iters.first;
		advance_if(_iters.first, _iters.second, predicate);
		return std::string(temp, _iters.first);
	}

	bool skip_empty_line() {
		skip_while(is_vspace);
		if (is_line_start())
		{
			auto pin = _iters.first;
			skip_while(is_hspace);
			if (eof() || is_newline(peek()))
				return true;

			_iters.first = pin;
		}

		return false;
	}

	lexical_token next_token() {
		while (skip_empty_line());
		if (is_line_start()) {
			if (auto indent = try_scan_indentation())
				return *indent;
		}

		skip_while(is_hspace);
		pin_location();

		if (!eof()) {
			auto cp = peek();

			if (is_newline(cp)) return next_token();
			if (is_quote(cp)) return scan_string_literal();
			if (is_digit(cp)) return scan_numeric_literal();
			if (is_ident_head(cp)) return scan_identifier();

			auto symbol = symbols::to_value({ static_cast<char>(cp) });
			if (symbol != symbols::unknown) { return scan_symbol(symbol); }

			skip(); // skip unknown token.
			return tok::make_error_token("unexpected token", location());
		}

		// close remaining indentation.
		// todo: fake the location changes.
		if (_indentation.depth() > 0) {
			_indentation.decrement();
			return tok::dedent();
		}

		return tok::eof();
	}


	std::optional<lexical_token> try_scan_indentation() {
		assert(!eof() && "unexpected end of source.");
		assert(is_line_start() && "expected start of line while attempting to scan indentation depth.");

		pin_location();
		auto indent = _indentation.measure(_iters.first, _iters.second);
		skip_while(is_hspace); // skip measured + remaining horizontal space.

		if (indent < _indentation) {
			_indentation = indent;
			return tok::dedent(location());
		}

		if (indent > _indentation) {
			_indentation = indent;
			return tok::indent(location());
		}

		return std::nullopt;
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


	lexical_token scan_string_literal() {
		assert(!eof() && "unexpected end of source.");
		assert(is_quote(peek()) && "expected a leading, double quotation mark, while attempting to scan a string literal.");

		auto prev = *_iters.first;
		auto escape = false;

		skip(); // consume quotation mark.

		auto temp = _iters.first;
		advance_if(_iters.first, _iters.second, [&](auto const& cp) {
			escape = (prev == '\\' && !escape);
			if (is_quote(cp) && !escape)
			{ prev = cp; return false; }
			prev = cp; return true;
		});

		if (!check(is_quote))
			return tok::make_error_token("expected closing \'\"\'");

		auto str = std::string(temp, _iters.first);
		skip(); // consume end quotation mark.
		return tok::string_literal(str, location());
	}

	lexical_token scan_numeric_literal() {
		assert(!eof() && "unexpected end of source.");
		assert(check('.') || check(is_digit) && "expected a leading digit while attempting to scan an integer literal.");

		if (is_zero_digit(peek())) {
			skip(); // consume zero digit.

			if (check('x') || check('X')) {
				skip(); // consume hexadecimal prefix.
				auto value = scan_while(is_hex_digit);
				return tok::integer_literal(std::stoll(value, 0, 16));
			}

			if (check('b') || check('B')) {
				skip(); // consume binary prefix
				auto value = scan_while(is_bin_digit);
				return tok::integer_literal(std::stoll(value, 0, 2));
			}

			if (check('.') && check(is_digit, 1)) {
				skip(); // consume decimal point.
				auto value = scan_while(is_digit);
				return tok::floating_literal(std::stod("." + value), location());
			}

			return tok::integer_literal(0, location());
		}

		auto integer_part = scan_while(is_digit);
		if (check('.') && check(is_digit, 1)) {
			skip(); // consume decimal point.
			auto fractional_part = scan_while(is_digit);
			return tok::floating_literal(std::stod(integer_part + "." + fractional_part), location());
		}

		assert(!integer_part.empty() && "expected an integer digit");
		return tok::integer_literal(std::stoll(integer_part), location());
	}


	lexical_token scan_symbol(symbol_t symbol) {
		assert(symbol != symbols::unknown && "unknown symbol");

		switch (symbol) {
			case symbols::ampersand: {
				if (check('&', 1)) { skip(2); return tok::logical_and(location()); }
				if (check('=', 1)) { skip(2); return tok::ampersand_equals(location()); }
			} break;

			case symbols::pipe: {
				if (check('|', 1)) { skip(2); return tok::logical_or(location()); }
				if (check('=', 1)) { skip(2); return tok::pipe_equals(location()); }
			} break;

			case symbols::plus: {
				if (check('+', 1)) { skip(2); return tok::increment(location()); }
				if (check('=', 1)) { skip(2); return tok::plus_equals(location()); }
			} break;

			case symbols::minus: {
				if (check('-', 1)) { skip(2); return tok::decrement(location()); }
				if (check('=', 1)) { skip(2); return tok::minus_equals(location()); }
			} break;

			case symbols::multiplication: {
				if (check('=', 1)) { skip(2); return tok::multiplication_equals(location()); }
			} break;

			case symbols::division: {
				if (check('=', 1)) { skip(2); return tok::division_equals(location()); }
			} break;

			case symbols::modulo: {
				if (check('=', 1)) { skip(2); return tok::modulo_equals(location()); }
			} break;

			case symbols::equals: {
				if (check('>', 1)) { skip(2); return tok::arrow(location()); }
				if (check('=', 1)) { skip(2); return tok::equality(location()); }
			} break;

			case symbols::exclamation_mark: {
				if (check('=', 1)) { skip(2); return tok::inequality(location()); }
			} break;

			case symbols::left_chevron: {
				if (check('=', 1)) { skip(2); return tok::left_chevron_equals(location()); }
			} break;

			case symbols::right_chevron: {
				if (check('=', 1)) { skip(2); return tok::right_chevron_equals(location()); }
			} break;

			case symbols::period: {
				if (check("..", 1)) { skip(3); return tok::ellipses(location()); }
				if (check(is_digit, 1)) return scan_numeric_literal();
			} break;

			default:;
		}

		skip(); // consume symbol.
		return tok::make_symbol_token(symbol, location());
	}
};

namespace rush {

	template <typename FwdIter>
	lexical_analysis lex(FwdIter first, FwdIter last, lexer_options const& opts) {
		auto l = lexer<FwdIter> { opts };
		l.tokenize(first, last);
		return { l.flush() };
	}

	lexical_analysis lex(char const* input, lexer_options const& opts) {
		auto first = input;
		auto last = input + std::strlen(input);
		return ::lex(first, last, opts);
	}

	lexical_analysis lex(std::string const& input, lexer_options const& opts) {
		auto first = begin(input);
		auto last = end(input);
		return ::lex(first, last, opts);
	}

	// FIXME: Wrap istreambuf_iterator in some kind of 'buffered_iterator'
	// promoting it to the required forward iterator type.
	lexical_analysis lex(std::istream& input, lexer_options const& opts) {
		auto first = std::istreambuf_iterator<char> { input };
		auto last = std::istreambuf_iterator<char> {};
		return ::lex(first, last, opts);
	}
} // namespace rush