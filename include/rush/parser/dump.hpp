#pragma once

#ifndef RUSH_PARSER_DUMP_HPP
#define RUSH_PARSER_DUMP_HPP

#include <iostream>
#include <string>

namespace rush::ast {
   class node;
}

namespace rush {
   class source;
   class syntax_analysis;
   class parser_options;

   void dump(ast::node const&, std::ostream& = std::cout);
   void dump(std::string const&, std::ostream& = std::cout);
   void dump(rush::source const&, std::ostream& = std::cout);
   void dump(std::istream&, std::ostream& = std::cout);

   void dump(std::string const&, parser_options const&, std::ostream& = std::cout);
   void dump(rush::source const&, parser_options const&, std::ostream& = std::cout);
   void dump(std::istream&, parser_options const&, std::ostream& = std::cout);
   void dump(rush::syntax_analysis const&, std::ostream& = std::cout);
} // rush

#endif // RUSH_PARSER_DUMP_HPP
