#pragma once

#ifndef RUSH_LEXER_TOKEN_HPP
#define RUSH_LEXER_TOKEN_HPP

#include <cstdlib>
#include <string>
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
		struct error_t {
			std::string _msg;
			friend bool operator == (error_t const& lhs, error_t const& rhs) {
				return lhs._msg == rhs._msg;
			}
		};

		struct identifier_t {
			std::string _text;
			friend bool operator == (identifier_t const& lhs, identifier_t const& rhs) {
				return lhs._text == rhs._text;
			}
		};

		struct integral_t {
			std::uint64_t _val;
			std::uint8_t _suffix;
			friend bool operator == (integral_t const& lhs, integral_t const& rhs) {
				return lhs._val == rhs._val;
			}
		};

		struct floating_t {
			double _val;
			std::uint8_t _suffix;
			friend bool operator == (floating_t const& lhs, floating_t const& rhs) {
				return lhs._val == rhs._val;
			}
		};

		using variant_type =
		std::variant<
			error_t,				// errors.
			symbol_t,         // symbols.
			keyword_t,        // keywords.
			identifier_t,     // identifiers.
			std::string, 		// string literals.
			std::uint64_t,    // integer literals.
			double>;          // floating literals. (todo: guarantee double is 64-bit)

		friend lexical_token tokens::make_error_token(std::string, location const&);
		friend lexical_token tokens::make_symbol_token(symbol_t, location const&);
		friend lexical_token tokens::make_keyword_token(keyword_t, location const&);

		friend lexical_token tokens::identifier(std::string, location const&);
		friend lexical_token tokens::string_literal(std::string, location const&);
		friend lexical_token tokens::integer_literal(std::uint64_t, location const&);
		friend lexical_token tokens::floating_literal(double, location const&);


		lexical_token(variant_type value, location const& loc)
			: _val(std::move(value)), _loc(loc) {}

		bool is_any() const noexcept { return false; }
		bool is_not_any() const noexcept { return true; }


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
				if constexpr (std::is_same_v<T, std::string>) return lexical_token_type::string_literal;
				if constexpr (std::is_same_v<T, std::uint64_t>) return lexical_token_type::integer_literal;
				if constexpr (std::is_same_v<T, double>) return lexical_token_type::floating_literal;
				assert("non-exhaustive visitor!");
			}, _val);
		}

		// \brief Returns true if the token is an instance of the specified symbol; false otherwise.
		bool is(symbol_t sym) const noexcept {
			if (auto pval = std::get_if<symbol_t>(&_val)) return *pval == sym;
			return false;
		}

		// \brief Returns true if the token is an instance of the specified keyword; false otherwise.
		bool is(keyword_t kw) const noexcept {
			if (auto pval = std::get_if<keyword_t>(&_val)) return *pval == kw;
			return false;
		}

		// \brief Returns true if the token matches any of the keywords or symbols passed; false otherwise.
		template <typename... Ts>
		bool is_any(keyword_t first, Ts... rest) const noexcept {
			return is(first) || is_any(rest...);
		}

		// \brief Returns true if the token matches any of the keywords or symbols passed; false otherwise.
		template <typename... Ts>
		bool is_any(symbol_t first, Ts... rest) const noexcept {
			return is(first) || is_any(rest...);
		}

		// \brief Returns true if the token is not an instance of the specified symbol; false otherwise.
		bool is_not(symbol_t sym) const noexcept {
			return !is(sym);
		}

		// \brief Returns true if the token is not an instance of the specified keyword; false otherwise.
		bool is_not(keyword_t kw) const noexcept {
			return !is(kw);
		}

		// \brief Returns true if the token does not matche any of the keywords or symbols passed; false otherwise.
		template <typename... Ts>
		bool is_not_any(keyword_t first, Ts... rest) const noexcept {
			return is_not(first) && is_not_any(rest...);
		}

		// \brief Returns true if the token does not matche any of the keywords or symbols passed; false otherwise.
		template <typename... Ts>
		bool is_not_any(symbol_t first, Ts... rest) const noexcept {
			return is_not(first) && is_not_any(rest...);
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

		bool is_string_literal() const noexcept {
			return std::holds_alternative<std::string>(_val);
		}

		bool is_integer_literal() const noexcept {
			return std::holds_alternative<std::uint64_t>(_val);
		}

		bool is_floating_literal() const noexcept {
			return std::holds_alternative<double>(_val);
		}

		bool is_boolean_literal() const noexcept {
			if (auto pval = std::get_if<keyword_t>(&_val))
				return *pval == keywords::true_ || *pval == keywords::false_;
				return false;
		}

		// \brief Returns true if the token is categorically a literal; false otherwise.
		bool is_literal() const noexcept {
			return is_string_literal()
				|| is_integer_literal()
				|| is_floating_literal()
				|| is_boolean_literal();
		}

		// \brief Returns true if this token shares the same value (as defined by the operator ==) with specified token.
		bool is_same(lexical_token const& other) const noexcept {
			return _val == other._val;
		}

	private:
		variant_type _val;
		struct location _loc;
	};


	inline bool operator == (lexical_token const& lhs, lexical_token const& rhs) {
		return lhs.location() == rhs.location() && lhs.is_same(rhs);
	}

	inline bool operator != (lexical_token const& lhs, lexical_token const& rhs) {
		return !(lhs == rhs);
	}

} // rush

#include "rush/lexer/_tokens.hpp"

#endif // RUSH_LEXER_TOKEN_HPP
