#pragma once

#ifndef RUSH_PARSER_PARSE_HPP
#define RUSH_PARSER_PARSE_HPP

#include "rush/parser/result.hpp"
#include "rush/parser/options.hpp"

#include <iostream>
#include <string>

namespace rush {
   class source;

	syntax_analysis parse(std::string const&, parser_options const& = {});
   syntax_analysis parse(rush::source const&, parser_options const& = {});
	syntax_analysis parse(std::istream&, parser_options const& = {});

} // rush

#endif // RUSH_PARSER_PARSE_HPP
