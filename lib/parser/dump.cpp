#include "rush/ast/node.hpp"
#include "rush/ast/printer.hpp"
#include "rush/parser/options.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"

namespace rush {

   void dump(std::string input, std::ostream& out) {
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

   void dump(rush::syntax_analysis const& sxa, std::ostream& out) {
      auto errs = sxa.errors();
      if (!errs.empty()) {
         for (auto& e : errs)
            out << "error " << e.location() << ": " << e.message() << std::endl;
      } else if (auto ast = sxa.ast()) {
         dump(*ast, out);
      }
   }

   void dump(rush::ast::node const& input, std::ostream& out) {
      input.accept(ast::printer { out });
   }
} // rush
