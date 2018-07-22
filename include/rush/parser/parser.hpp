#pragma once

#ifndef RUSH_PARSER_PARSER_HPP
#define RUSH_PARSER_PARSER_HPP

#include <string>
#include <iostream>

#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"
#include "rush/ast/abstract_syntax_tree.hpp"

namespace rush {

	struct parser_options { };

	abstract_syntax_tree parse(std::string, parser_options const& = {});
	abstract_syntax_tree parse(std::istream&, parser_options const& = {});


} // rush

#endif // RUSH_PARSER_PARSER_HPP
