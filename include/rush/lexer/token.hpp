#pragma once

#ifndef RUSH_LEXER_TOKEN_HPP
#define RUSH_LEXER_TOKEN_HPP

#include <cstdlib>
#include <string>
#include <string_view>
#include <variant>

// #include "rush/lexer/_symbols.hpp.gen"
// #include "rush/lexer/_keywords.hpp.gen"

namespace rush {

	enum class symbol_t {};
	enum class keyword_t {};

	using line_index_t = std::size_t;
	using column_index_t = std::size_t;

	enum class lexical_token_type {
		symbol,
		keyword,
		identifier,
		string_literal,
		integer_literal,
		floating_literal,
	};

	struct lexical_token_info {
		line_index_t line() const noexcept { return _line; }
		column_index_t column() const noexcept { return _column; }

		lexical_token_info(line_index_t ln, column_index_t col)
			: _line(ln)
			, _column(col) {}

	private:
		line_index_t const _line;
		column_index_t const _column;
	};

	struct lexical_token_value {

		explicit lexical_token_value(symbol_t) {}
		explicit lexical_token_value(keyword_t) {}
		explicit lexical_token_value(std::string) {}
		explicit lexical_token_value(std::uint64_t) {}
		explicit lexical_token_value(double) {}

	private:
		std::variant<
			symbol_t,
			keyword_t,
			std::string,
			std::uint64_t,
			double> _val; // todo: guarantee double is 64-bit
	};

	class lexical_token {
	public:
		lexical_token(
			lexical_token_value,
			lexical_token_info);

		std::size_t size() const noexcept;
		std::string_view raw() const noexcept;
		std::string_view text() const noexcept;

		line_index_t line() const noexcept { return _info.line(); }
		column_index_t column() const noexcept { return _info.column(); }

		lexical_token_type type() const noexcept { return _type; }
		lexical_token_info const& info() const noexcept { return _info; }


		// \brief Returns true if the token is an instance of the specified symbol; false otherwise.
		bool is(symbol_t kw) const noexcept;

		// \brief Returns true if the token is an instance of the specified keyword; false otherwise.
		bool is(keyword_t kw) const noexcept;

		// \brief Returns true if the token is not an instance of the specified symbol; false otherwise.
		bool is_not(symbol_t kw) const noexcept;

		// \brief Returns true if the token is not an instance of the specified keyword; false otherwise.
		bool is_not(keyword_t kw) const noexcept;

		// \brief Returns true if the token is categorically a symbol; false otherwise.
		bool is_symbol() const noexcept {
			return type() == lexical_token_type::symbol;
		}

		// \brief Returns true if the token is categorically a keyword; false otherwise.
		bool is_keyword() const noexcept {
			return type() == lexical_token_type::keyword;
		}

		// \brief Returns true if the token is categorically an identifier; false otherwise.
		bool is_identifier() const noexcept {
			return type() == lexical_token_type::identifier;
		}

		// \briefe Returns true if the token is categorically a literal; false otherwise.
		bool is_literal() const noexcept {
			switch (type()) {
				case lexical_token_type::integer_literal:
				case lexical_token_type::floating_literal:
				case lexical_token_type::string_literal: return true;
				default: return false;
			}
		}

	private:
		lexical_token_type _type;
		lexical_token_info _info;
		lexical_token_value _value;
	};

} // rush

// #include "_tokens.hpp.gen"

#endif // RUSH_LEXER_TOKEN_HPP
