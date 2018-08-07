#pragma once

#ifndef RUSH_PARSER_PARSE_HPP
#define RUSH_PARSER_PARSE_HPP

#include <string>
#include <iostream>

#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"

#include "rush/lexer/lex.hpp"
#include "rush/parser/ast.hpp"


#include <iostream>
#include <string>
#include <memory>

namespace rush {

	// lexer_options is intended to be used
	// as a mixin here; not a base class.
	struct parser_options : public lexer_options {

	};

	abstract_syntax_tree parse(std::string, parser_options const& = {});
	abstract_syntax_tree parse(std::istream&, parser_options const& = {});

	void dump(std::ostream&, abstract_syntax_tree const&);
	void print(std::ostream&, abstract_syntax_tree const&);

} // rush

#endif // RUSH_PARSER_PARSE_HPP
