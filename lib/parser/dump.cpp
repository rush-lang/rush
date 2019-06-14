#include "rush/core/source.hpp"
#include "rush/diag/dump.hpp"
#include "rush/ast/node.hpp"
#include "rush/ast/printer.hpp"
#include "rush/parser/options.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"

namespace rush {

   void dump(std::string const& input, std::ostream& out) {
      dump(parse(input), out);
   }

   void dump(rush::source const& input, std::ostream& out) {
      dump(parse(input), out);
   }

   void dump(std::istream& input, std::ostream& out) {
      dump(parse(input), out);
   }

   void dump(std::string input, parser_options const& opts, std::ostream& out) {
      dump(parse(input, opts), out);
   }

   void dump(std::istream& input, parser_options const& opts, std::ostream& out) {
      dump(parse(input, opts), out);
   }

   void dump(rush::source const& input, parser_options const& opts, std::ostream& out) {
      dump(parse(input, opts), out);
   }

   void dump(rush::syntax_analysis const& sxa, std::ostream& out) {
      for (auto& e : sxa.errors())
         rush::dump(*e, out);

      if (sxa.errors().empty())
         sxa.accept(ast::printer { out });
   }

   void dump(rush::ast::node const& input, std::ostream& out) {
      input.accept(ast::printer { out });
   }
} // rush
