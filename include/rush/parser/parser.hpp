#pragma once

#ifndef RUSH_PARSER_PARSER_HPP
#define RUSH_PARSER_PARSER_HPP

#include <string>
#include <iostream>

#include "rush/ast/ast.hpp"

namespace rush {

	struct parser_options { };

	abstract_syntax_tree parse(std::string, parser_options const& = {});
	abstract_syntax_tree parse(std::istream&, parser_options const& = {});

	void dump(std::istream&, abstract_syntax_tree const&);
	void print(std::istream&, abstract_syntax_tree const&);
} // rush

#endif // RUSH_PARSER_PARSER_HPP
