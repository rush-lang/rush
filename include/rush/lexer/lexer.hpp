#pragma once

#ifndef RUSH_LEXER_LEXER_HPP
#define RUSH_LEXER_LEXER_HPP

#include <string>
#include <iostream>

namespace rush {
	class lexer_options {

	};

	class lexical_analysis_iterator {

	};

	class lexical_analysis final {
		friend lexical_analysis lex(std::string const&, lexer_options const&);
		friend lexical_analysis lex(std::istream&, lexer_options const&);

	public:
		lexical_analysis(lexical_analysis const&) = delete;
		void operator = (lexical_analysis const&) = delete;

		typedef lexical_analysis_iterator iterator;
		typedef lexical_analysis_iterator const_iterator;

		bool empty() const noexcept;
		std::size_t size() const noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

	private:
		lexical_analysis() = default;

		template <typename TokenRange, typename ErrorRange>
		lexical_analysis(TokenRange&&, ErrorRange&&);
	};

	lexical_analysis lex(char const*, lexer_options const& = {});
	lexical_analysis lex(std::string const&, lexer_options const& = {});
	lexical_analysis lex(std::istream&, lexer_options const& = {});
}

#endif // RUSH_LEXER_LEXER_HPP
