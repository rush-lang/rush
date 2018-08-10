#pragma once

#ifndef RUSH_PARSER_PARSE_HPP
#define RUSH_PARSER_PARSE_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"

#include "rush/lexer/lex.hpp"
#include "rush/parser/result.hpp"

#include <iostream>
#include <string>
#include <memory>

namespace rush {

	// lexer_options is intended to be used
	// as a mixin here; not a base class.
	struct parser_options : public lexer_options {

	};

	parse_result parse(std::string, parser_options const& = {});
	parse_result parse(std::istream&, parser_options const& = {});

	void dump(std::ostream&, parse_result const&);
	void print(std::ostream&, parse_result const&);

} // rush

#endif // RUSH_PARSER_PARSE_HPP
