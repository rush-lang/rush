#pragma once

#ifndef RUSH_LEXER_LEXER_HPP
#define RUSH_LEXER_LEXER_HPP

#include "rush/core/extras.hpp"
#include "rush/lexer/token.hpp"
#include "rush/lexer/lex.hpp"
#include "rush/lexer/lookahead.hpp"
#include "rush/lexer/indentation.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <iosfwd>
#include <iterator>
#include <optional>

namespace rush {
	class parser;
	class source;
	class lexer;

	template <typename InIter>
	class lexer_iterator;

	class lexer {
		friend class lexical_iterator;

		lexer(lexer const&) = delete;
		void operator = (lexer const&) = delete;

	public:
		explicit lexer(lexer_options opts = {}) noexcept
			: _opts(opts) {}

		lexer(lexer&& other) = default;
		lexer& operator = (lexer&& other) = default;

		lexical_analysis tokenize(std::string);
		lexical_analysis tokenize(std::istream&);

	private:
		using lookahead_buffer_type = rush::lookahead_buffer<
			lexer_iterator<std::istreambuf_iterator<char>>, 8>;

		lexer_options _opts;
		std::unique_ptr<lookahead_buffer_type> _lab;
		rush::indentation _indent;
		rush::location _loc; // location pinned before every new token.

		bool eof() const noexcept;
		bool is_line_start() const noexcept;

		rush::location location() const noexcept;

		bool check(codepoint_t cp, std::size_t offset = 0);
		bool icheck(codepoint_t cp, std::size_t offset = 0);
		bool check(std::string str, std::size_t offset = 0);
		bool icheck(std::string str, std::size_t offset = 0);

		auto check_if(function_ref<bool(codepoint_t)> predicate, std::size_t offset = 0)
			-> decltype(predicate(codepoint_t{}), bool{}) {
			return !eof() && predicate(peek(offset));
		}

		codepoint_t peek(std::size_t offset = 0);
		codepoint_t skip(std::size_t offset = 1);
		std::string scan(std::size_t count);

		codepoint_t skip_while(function_ref<bool(codepoint_t)>);
		std::string scan_while(function_ref<bool(codepoint_t)>);

		bool check(codepoint_t, std::size_t offset) const noexcept;
		bool icheck(codepoint_t, std::size_t offset) const noexcept;

		lexical_token next_token();

		std::optional<lexical_token> scan_whitespace();

		lexical_token scan_identifier();
		lexical_token scan_string_literal();

		lexical_token_suffix scan_floating_literal_suffix();
		lexical_token_suffix scan_integer_literal_suffix();
		lexical_token scan_integer_literal(function_ref<bool(codepoint_t)>, int base);
		lexical_token scan_numeric_literal();

		lexical_token scan_symbol(symbol_token_t symbol);

		lexical_token _unwind_indentation();
	};
};

#endif // RUSH_LEXER_LEXER_HPP
