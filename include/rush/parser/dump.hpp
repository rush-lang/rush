#pragma once

#ifndef RUSH_PARSER_DUMP_HPP
#define RUSH_PARSER_DUMP_HPP

#include <iostream>
#include <string>

namespace rush::ast {
   class node;
}

namespace rush {
   void dump(ast::node const& input);
   void dump(ast::node const& input, std::ostream& out);

	class parse_result;
	void dump(parse_result const& input);
	void dump(parse_result const& input, std::ostream& out);

	class parser_options;
	void dump(std::string input, std::ostream& out = std::cout);
	void dump(std::istream& input, std::ostream& out = std::cout);
	void dump(std::string input, parser_options const&, std::ostream& out = std::cout);
	void dump(std::istream& input, parser_options const&, std::ostream& out = std::cout);
} // rush

#endif // RUSH_PARSER_DUMP_HPP
