#pragma once

#ifndef RUSH_LEXER_TOKEN_HPP
#define RUSH_LEXER_TOKEN_HPP

#include <cstdlib>
#include <string>
#include <string_view>
#include <variant>

#include "rush/lexer/_symbols.hpp"
#include "rush/lexer/_keywords.hpp"

namespace rush {

	enum class lexical_token_type {
		symbol,
		keyword,
		identifier,
		string_literal,
		integer_literal,
		floating_literal,
	};


	class lexical_token final {
	public:
		explicit lexical_token(symbol_t) {}
		explicit lexical_token(keyword_t) {}
		explicit lexical_token(std::string) {}
		explicit lexical_token(std::uint64_t) {}
		explicit lexical_token(double) {}

		// \brief Returns the length of the token
		std::size_t size() const noexcept;

		// \brief Returns the length of the token.
		std::size_t length() const noexcept;

		// \brief Returns the plain text of the token.
		std::string_view text() const noexcept;

		// \brief Returns the categorical type of the token based on its value.
		lexical_token_type type() const noexcept { return _type; }

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

		// \brief Returns true if the token is categorically a literal; false otherwise.
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
		std::variant<
			symbol_t,
			keyword_t,
			std::string,
			std::uint64_t,
			double> _val; // todo: guarantee double is 64-bit
	};

} // rush

#include "rush/lexer/_tokens.hpp"

#endif // RUSH_LEXER_TOKEN_HPP
