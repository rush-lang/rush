#pragma once

#ifndef RUSH_PARSER_PARSE_HPP
#define RUSH_PARSER_PARSE_HPP

#include "rush/sema/scope.hpp"

#include "rush/ast/declaration.hpp"
#include "rush/ast/statement.hpp"
#include "rush/ast/expression.hpp"

#include "rush/lexer/lex.hpp"
#include "rush/parser/result.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <optional>

namespace rush {

	// lexer_options is intended to be used
	// as a mixin here; not a base class.
	struct parser_options : public lexer_options {
		rush::scope* scope;

		parser_options()
			: scope(&global_scope) {}
	};

	parse_result parse(std::string, parser_options const& = {});
	parse_result parse(std::istream&, parser_options const& = {});

	void dump(std::string input, std::ostream& out = std::cout);
	void dump(std::istream& input, std::ostream& out = std::cout);

	void dump(std::string input, parser_options, std::ostream& out = std::cout);
	void dump(std::istream& input, parser_options, std::ostream& out = std::cout);

	void dump(parse_result const& input, std::ostream& out = std::cout);
} // rush

#endif // RUSH_PARSER_PARSE_HPP
