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

#endif // RUSH_LEXER_LEXER_HPP
