#pragma once

#ifndef RUSH_PARSER_PARSE_HPP
#define RUSH_PARSER_PARSE_HPP

#include "rush/parser/result.hpp"

#include <iostream>
#include <string>

namespace rush {
	class parser_options;

	syntax_analysis parse(std::string);
	syntax_analysis parse(std::istream&);

	syntax_analysis parse(std::string, parser_options const&);
	syntax_analysis parse(std::istream&, parser_options const&);
} // rush

#endif // RUSH_PARSER_PARSE_HPP
