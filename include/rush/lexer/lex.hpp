#pragma once

#ifndef RUSH_LEXER_LEX_HPP
#define RUSH_LEXER_LEX_HPP

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"

#include <string>
#include <iostream>

namespace rush {
	struct lexer_options {

	};

	lexical_analysis lex(char const*, lexer_options const& = {});
	lexical_analysis lex(std::string const&, lexer_options const& = {});
	lexical_analysis lex(std::istream&, lexer_options const& = {});
}

#endif // RUSH_LEXER_LEX_HPP
