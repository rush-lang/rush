#pragma once

#ifndef RUSH_PARSER_PARSE_HPP
#define RUSH_PARSER_PARSE_HPP

#include "rush/parser/result.hpp"

#include <iostream>
#include <string>

namespace rush {
	class parser_options;

	parse_result parse(std::string);
	parse_result parse(std::istream&);

	parse_result parse(std::string, parser_options const&);
	parse_result parse(std::istream&, parser_options const&);
} // rush

#endif // RUSH_PARSER_PARSE_HPP
