#pragma once

#ifndef RUSH_LEXER_LEXER_HPP
#define RUSH_LEXER_LEXER_HPP

#include "rush/lexer/token.hpp"
#include "rush/lexer/options.hpp"
#include "rush/lexer/analysis.hpp"

#include <string>
#include <iostream>

namespace rush {
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
	void skip_line_comment(InIter& first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		assert(*first++ == '/' && "expected a leading forward slash");

		if (*first == '/')
			advance_if(first, last, [](auto const& cp) { return !charinfo::is_vspace(cp); });
		skip_vspace(first, last);
	}
} // rush

#endif // RUSH_LEXER_LEXER_HPP
