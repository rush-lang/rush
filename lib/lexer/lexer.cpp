#include "rush/core/ascii.hpp"
#include "rush/core/algorithm.hpp"
#include "rush/core/iterator.hpp"
#include "rush/lexer/lexer.hpp"
#include "rush/lexer/lexer_iterator.hpp"

#include <iostream>
#include <sstream>

using namespace rush::ascii;
namespace tok = rush::tokens;

namespace rush {

	lexical_analysis lex(char const* input, lexer_options const& opts) {
		auto l = lexer { opts };
		return l.tokenize(input);
	}

	lexical_analysis lex(std::string const& input, lexer_options const& opts) {
		auto l = lexer { opts };
		return l.tokenize(input);
	}

	lexical_analysis lex(std::istream& input, lexer_options const& opts) {
		auto l = lexer { opts };
		return l.tokenize(input);
	}
} // rush

namespace rush {
	bool lexer::eof() const noexcept {
		return _lab->empty();
	}

	bool lexer::is_line_start() const noexcept {
		return !eof() && _lab->peek().location().column() == 1;
	}

	rush::location lexer::location() const noexcept {
		return _loc;
	}

	bool lexer::check(codepoint_t cp, std::size_t offset) {
		return !eof() && peek(offset) == cp;
	}

	bool lexer::icheck(codepoint_t cp, std::size_t offset) {
		return !eof() && iequal(peek(offset), cp);
	}

	bool lexer::check(std::string str, std::size_t offset) {
		for (std::size_t i = 0; i < str.length(); ++i)
			if (!check(str[i], offset + i)) return false;
		return true;
	}

	bool lexer::icheck(std::string str, std::size_t offset) {
		for (std::size_t i = 0; i < str.length(); ++i)
			if (!icheck(str[i], offset + i)) return false;
		return true;
	}

	codepoint_t lexer::peek(std::size_t offset) {
		assert(!eof() && "unexpected end of source.");
		return offset < _lab->size()
			? _lab->peek(offset).elem()
			: npos;
	}

	codepoint_t lexer::skip(std::size_t offset) {
		assert(!eof() && "unexpected end of source.");
		_lab->skip(offset);
		return !eof() ? peek() : npos;
	}

	std::string lexer::scan(std::size_t count) {
		assert(!eof() && "unexpected end of source.");
		std::string result;
		for (; !eof() && count > 0; --count)
			result.push_back(_lab->next().elem());
		return std::move(result);
	}

	codepoint_t lexer::skip_while(function_ref<bool(codepoint_t)> pred) {
		// assert(!eof() && "unexpected end of source.");
		auto cp = npos;
		while (!eof() && pred(cp = _lab->peek().elem())) _lab->skip();
		return cp;
	}

	std::string lexer::scan_while(function_ref<bool(codepoint_t)> pred) {
		// assert(!eof() && "unexpected end of source.");
		std::string result;
		while (!eof() && pred(peek()))
			result.push_back(_lab->next().elem());
		return std::move(result);
	}

	lexical_analysis lexer::tokenize(std::string str) {
		std::istringstream iss { str };
		return tokenize(iss);
	}

	lexical_analysis lexer::tokenize(std::istream& istr) {
		auto toks = std::vector<lexical_token> {};
		auto first = std::istreambuf_iterator<char> { istr };
		auto last = std::istreambuf_iterator<char> { };
		_lab = std::make_unique<lookahead_buffer_type>(
			lexer_iterator { first, *first, {} },
			lexer_iterator { last, 0, rush::location::undefined});
		_indent = {};

		if (!eof()) {
			auto tok = next_token();
			for (; !tok.is(symbols::eof); tok = next_token())
				toks.push_back(std::move(tok));
		}

		return lexical_analysis { std::move(toks) };
	}

	lexical_token lexer::next_token() {
		auto indent = scan_whitespace();
		if (indent) return *indent;

		if (!eof()) {
			_loc = _lab->peek().location(); // pin the location
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

		return _unwind_indentation();
	}

	std::optional<lexical_token> lexer::scan_whitespace() {
		while (!eof()) {
			skip_while(is_vspace);
			if (eof()) break;

			if (is_line_start()) {
				_loc = _lab->peek().location(); // pin the location
				auto wspace = scan_while(is_hspace);
				if (eof()) break;

				if (!is_newline(peek())) {
					auto indent = _indent.measure(wspace.begin(), wspace.end());
					if (indent < _indent) { _indent.decrement(); return tok::dedent(location()); }
					if (indent > _indent) { _indent.increment(); return tok::indent(location()); }
					break;
				}
			} else {
				skip_while(is_hspace);
				if (eof() || !is_vspace(peek()))
					break;
			}
		}
		return std::nullopt;
	}

	lexical_token lexer::scan_identifier() {
		assert(!eof() && "unexpected end of source.");
		assert(is_ident_head(peek()) && "expected a leading identifier character while attempting to scan an identifier.");

		auto ident = scan_while(is_ident_body);
		auto kw_val = keywords::to_value(ident);
		return kw_val != keywords::unknown
			? tok::make_keyword_token(kw_val, location())
			: tok::identifier(ident, location());
	}


	lexical_token lexer::scan_string_literal() {
		assert(!eof() && "unexpected end of source.");
		assert(is_quote(peek()) && "expected a leading, double quotation mark, while attempting to scan a string literal.");

		auto prev = peek();
		auto escape = false;

		skip(); // consume quotation mark.
		auto result = scan_while([&](auto const& cp){
			escape = (prev == '\\' && !escape);
			if (is_quote(cp) && !escape)
			{ prev = cp; return false; }
			prev = cp; return true;
		});

		if (!check_if(is_quote))
			return tok::make_error_token("expected closing \'\"\'");

		skip(); // consume end quotation mark.
		return tok::string_literal(std::move(result), location());
	}

	lexical_token_suffix lexer::scan_floating_literal_suffix() {
		if (icheck('f')) { skip(); return lexical_token_suffix::float_literal; }
		return lexical_token_suffix::none;
	}

	lexical_token_suffix lexer::scan_integer_literal_suffix() {
		if (icheck('u')) { skip(); return lexical_token_suffix::unsigned_literal; }
		if (icheck('l')) { skip(); return lexical_token_suffix::long_literal; }

		// if (icheck("ul")) {
		// 	skip(2);
		// 	return
		// 		lexical_token_suffix::long_literal |
		// 		lexical_token_suffix::unsigned_literal;
		// }

		return lexical_token_suffix::none;
	}

	lexical_token lexer::scan_integer_literal(function_ref<bool(codepoint_t)> predicate, int base) {
		auto value = scan_while(predicate);
		auto suffix = scan_integer_literal_suffix();
		return tok::suffixed_integer_literal(std::stoll(value, 0, base), suffix, location());
	}

	lexical_token lexer::scan_numeric_literal() {
		assert(!eof() && "unexpected end of source.");
		assert(check('.') || check_if(is_digit) && "expected a leading digit while attempting to scan an integer literal.");

		if (is_zero_digit(peek())) {
			skip(); // consume zero digit.

			if (icheck('x')) { skip(); return scan_integer_literal(is_hex_digit, 16); }
			if (icheck('b')) { skip(); return scan_integer_literal(is_bin_digit, 2); }
			if (icheck('o')) { skip(); return scan_integer_literal(is_oct_digit, 8); }

			if (check('.') && !check("..", 1)) {
				skip(); // consume decimal point.
				auto value = scan_while(is_digit);
				auto suffix = scan_floating_literal_suffix();
				return tok::suffixed_floating_literal(std::stod("0." + value), suffix, location());
			}

			auto suffix = scan_integer_literal_suffix();
			return tok::suffixed_integer_literal(0, suffix, location());
		}

		auto integer_part = scan_while(is_digit);
		if (check('.') && !check("..", 1)) {
			skip(); // consume decimal point.
			auto fractional_part = scan_while(is_digit);
			auto suffix = scan_floating_literal_suffix();
			return tok::suffixed_floating_literal(
				std::stod(integer_part + "." + fractional_part),
				suffix, location());
		}

		assert(!integer_part.empty() && "expected an integer digit");
		auto suffix = scan_integer_literal_suffix();
		return tok::suffixed_integer_literal(
			std::stoll(integer_part),
			suffix, location());
	}


	lexical_token lexer::scan_symbol(symbol_token_t symbol) {
		assert(symbol != symbols::unknown && "unknown symbol");

		switch (symbol) {
         case symbols::ampersand: {
				if (check('&', 1)) { skip(2); return tok::double_ampersand(location()); }
				if (check('=', 1)) { skip(2); return tok::ampersand_equals(location()); }
			} break;

			case symbols::pipe: {
				if (check('|', 1)) { skip(2); return tok::double_pipe(location()); }
				if (check('=', 1)) { skip(2); return tok::pipe_equals(location()); }
			} break;

			case symbols::plus: {
				if (check('+', 1)) { skip(2); return tok::plus_plus(location()); }
				if (check('=', 1)) { skip(2); return tok::plus_equals(location()); }
			} break;

			case symbols::minus: {
				if (check('-', 1)) { skip(2); return tok::minus_minus(location()); }
				if (check('=', 1)) { skip(2); return tok::minus_equals(location()); }
			} break;

         case symbols::caret: {
            if (check('=', 1)) { skip(2); return tok::caret_equals(location()); }
         } break;

			case symbols::asterisk: {
				if (check('=', 1)) { skip(2); return tok::asterisk_equals(location()); }
			} break;

			case symbols::forward_slash: {
				if (check('=', 1)) { skip(2); return tok::forward_slash_equals(location()); }
			} break;

			case symbols::percent: {
				if (check('=', 1)) { skip(2); return tok::percent_equals(location()); }
			} break;

			case symbols::equals: {
				if (check('>', 1)) { skip(2); return tok::arrow(location()); }
				if (check('=', 1)) { skip(2); return tok::equals_equals(location()); }
			} break;

			case symbols::exclamation_mark: {
				if (check('=', 1)) { skip(2); return tok::exclamation_mark_equals(location()); }
			} break;

			case symbols::left_chevron: {
				if (check('=', 1)) { skip(2); return tok::left_chevron_equals(location()); }
				if (check('<', 1)) { skip(2);
				   if (check('=')) { skip(1); return tok::double_left_chevron_equals(location()); }
               return tok::double_left_chevron(location());
            }
			} break;

			case symbols::right_chevron: {
				if (check('=', 1)) { skip(2); return tok::right_chevron_equals(location()); }
            if (check('>', 1)) { skip(2);
				   if (check('=')) { skip(1); return tok::double_right_chevron_equals(location()); }
               return tok::double_right_chevron(location());
            }
			} break;

			case symbols::period: {
				if (check("..", 1)) { skip(3); return tok::ellipses(location()); }
				if (check_if(is_digit, 1)) return scan_numeric_literal();
			} break;

			default:;
		}

		skip(); // consume symbol.
		return tok::make_symbol_token(symbol, location());
	}

	lexical_token lexer::_unwind_indentation() {
		if (_indent.depth() > 0) {
			_loc = rush::location {
				_loc.line() + 1,
				_indent.depth()
			};

			_indent.decrement();
			return tok::dedent(location());
		}
		return tok::eof(location());
	}
}
