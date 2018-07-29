#pragma once

#ifndef RUSH_LEXER_TOKEN_HPP
#define RUSH_LEXER_TOKEN_HPP

#include <cstdlib>
#include <string>
#include <string_view>
#include <variant>

#include "rush/lexer/_symbols.hpp"
#include "rush/lexer/_keywords.hpp"
#include "rush/core/location.hpp"

namespace rush {

	class lexical_token;

	enum class lexical_token_type {
		error,
		symbol,
		keyword,
		identifier,
		string_literal,
		integer_literal,
		floating_literal,
	};

	namespace tokens {
		inline lexical_token make_error_token(std::string, location const& = location::undefined);
		inline lexical_token make_symbol_token(symbol_t, location const& = location::undefined);
		inline lexical_token make_keyword_token(keyword_t, location const& = location::undefined);

		inline lexical_token identifier(std::string, location const& = location::undefined);
		inline lexical_token string_literal(std::string, location const& = location::undefined);
		inline lexical_token integer_literal(std::uint64_t, location const& = location::undefined);
		inline lexical_token floating_literal(double, location const& = location::undefined);
	}

	class lexical_token final {
		struct error_t { std::string_view _msg; };
		struct identifier_t { std::string_view _text; };

		using variant_type =
		std::variant<
			error_t,				// errors.
			symbol_t,         // symbols.
			keyword_t,        // keywords.
			identifier_t,     // identifiers.
			std::string_view, // string literals.
			std::uint64_t,    // integer literals.
			double>;          // floating literals. (todo: guarantee double is 64-bit)

		friend lexical_token tokens::make_error_token(std::string, location const&);
		friend lexical_token tokens::make_symbol_token(symbol_t, location const&);
		friend lexical_token tokens::make_keyword_token(keyword_t, location const&);

		friend lexical_token tokens::identifier(std::string_view, location const&);
		friend lexical_token tokens::string_literal(std::string_view, location const&);
		friend lexical_token tokens::integer_literal(std::uint64_t, location const&);
		friend lexical_token tokens::floating_literal(double, location const&);


		lexical_token(variant_type value, location const& loc)
			: _val(std::move(value)), _loc(loc) {}

	public:

		// \brief Returns the length of the token
		std::size_t size() const noexcept {
			return this->text().size();
		}

		// \brief Returns the length of the token.
		std::size_t length() const noexcept {
			return this->text().size();
		}

		// \brief Returns the location in source of the token.
		location const& location() const noexcept {
			return this->_loc;
		}

		// \brief Returns the plain text of the token.
		std::string text() const noexcept;

		// \brief Returns the categorical type of the token based on its value.
		lexical_token_type type() const noexcept {
			return std::visit([](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;

				if constexpr (std::is_same_v<T, error_t>) return lexical_token_type::error;
				if constexpr (std::is_same_v<T, symbol_t>) return lexical_token_type::symbol;
				if constexpr (std::is_same_v<T, keyword_t>) return lexical_token_type::keyword;
				if constexpr (std::is_same_v<T, identifier_t>) return lexical_token_type::identifier;
				if constexpr (std::is_same_v<T, std::string_view>) return lexical_token_type::string_literal;
				if constexpr (std::is_same_v<T, std::uint64_t>) return lexical_token_type::integer_literal;
				if constexpr (std::is_same_v<T, double>) return lexical_token_type::floating_literal;
				assert("non-exhaustive visitor!");
			}, _val);
		}

		// \brief Returns true if the token is an instance of the specified symbol; false otherwise.
		bool is(symbol_t sym) const noexcept {
			return is_symbol() && std::get<symbol_t>(_val) == sym;
		}

		// \brief Returns true if the token is an instance of the specified keyword; false otherwise.
		bool is(keyword_t kw) const noexcept {
			return is_keyword() && std::get<keyword_t>(_val) == kw;
		}

		// \brief Returns true if the token is not an instance of the specified symbol; false otherwise.
		bool is_not(symbol_t sym) const noexcept {
			return !is(sym);
		}

		// \brief Returns true if the token is not an instance of the specified keyword; false otherwise.
		bool is_not(keyword_t kw) const noexcept {
			return !is(kw);
		}

		bool is_error() const noexcept {
			return std::holds_alternative<error_t>(_val);
		}

		// \brief Returns true if the token is categorically a symbol; false otherwise.
		bool is_symbol() const noexcept {
			return std::holds_alternative<symbol_t>(_val);
		}

		// \brief Returns true if the token is categorically a keyword; false otherwise.
		bool is_keyword() const noexcept {
			return std::holds_alternative<keyword_t>(_val);
		}

		// \brief Returns true if the token is categorically an identifier; false otherwise.
		bool is_identifier() const noexcept {
			return std::holds_alternative<identifier_t>(_val);
		}

		// \brief Returns true if the token is categorically a literal; false otherwise.
		bool is_literal() const noexcept {
			return std::holds_alternative<std::string_view>(_val)
				|| std::holds_alternative<std::uint64_t>(_val)
				|| std::holds_alternative<double>(_val);
		}

		bool is_same(lexical_token const& other) const noexcept {
			return type() == other.type() && compare_value(other) == 0;
		}

	private:
		variant_type _val;
		struct location _loc;

		int compare_value(lexical_token const& other) const noexcept {
			assert(type() == other.type() && "cannot compare values of differing token types.");
			return 0;
		}
	};

} // rush

#include "rush/lexer/_tokens.hpp"

#endif // RUSH_LEXER_TOKEN_HPP
