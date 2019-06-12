#pragma once

#ifndef RUSH_LEXER_LEX_HPP
#define RUSH_LEXER_LEX_HPP

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"

#include <iostream>
#include <string_view>

namespace rush {
   class source;
	struct lexer_options {

	};

   lexical_analysis lex(std::istream&, lexer_options const& = {});
	lexical_analysis lex(std::string_view, lexer_options const& = {});
	lexical_analysis lex(std::istream&, std::string_view id, lexer_options const& = {});
	lexical_analysis lex(std::string_view, std::string_view id, lexer_options const& = {});
   lexical_analysis lex(rush::source const&, lexer_options const& = {});
}

#endif // RUSH_LEXER_LEX_HPP
