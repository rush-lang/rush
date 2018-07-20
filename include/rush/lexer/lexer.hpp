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
		friend lexical_analysis lex(char const*, lexer_options const&);
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


// ## THE BELOW RESIDES IN THIS HEADER TEMPMORARILY WHILE DOING TDD,
// ## WILL BE MOVED INTO THE CPP FILE WHEN READY.
#include "rush/core/charinfo.hpp"
#include "rush/core/iterator.hpp"

namespace rush {
	template <typename InIter>
	void skip_hspace(InIter& first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		advance_if(first, last, charinfo::is_hspace);
	}

	template <typename InIter>
	void skip_vspace(InIter& first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		advance_if(first, last, charinfo::is_vspace);
	}

	template <typename InIter>
	InIter scan_integer_literal(InIter first, InIter const& last) {
		assert(first != last && charinfo::is_digit(*first)
			&& "expected a leading digit while attempting to scan an integer literal.");

		if (charinfo::is_zero_digit(*first)) return ++first;
		advance_if(first, last, charinfo::is_digit);
		return first;
	}

	template <typename InIter>
	InIter scan_floating_literal(InIter first, InIter const& last) {
		assert(first != last && (charinfo::is_digit(*first) || *first == '.')
			&& "expected a leading digit, or dot, while attempting to scan a floating literal.");

		return first;
	}

	template <typename InIter>
	InIter scan_string_literal(InIter first, InIter const& last) {
		assert(first != last && charinfo::is_quote(*first)
			&& "expected a leading, double quotation mark, while attempting to scan a string literal.");

		auto prev = *first;
		advance_if(++first, last, [&prev](auto& cp) {
			if (prev != '\\' && charinfo::is_quote(cp)) return false;
			prev = cp;
			return true;
		});

		if (charinfo::is_quote(*first)) ++first;
		return first;
	}

	template <typename InIter>
	InIter scan_identifier(InIter first, InIter const& last) {
		assert(first != last && charinfo::is_ident_head(*first));
		advance_if(first, last, charinfo::is_ident_body);
		return first;
	}
}

#endif // RUSH_LEXER_LEXER_HPP
