/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#include "rush/core/source.hpp"
#include "rush/diag/dump.hpp"
#include "rush/ast/node.hpp"
#include "rush/ast/printer.hpp"
#include "rush/parser/options.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"

namespace rush {

   void dump(std::string const& input, std::ostream& out) {
      ast::context ctx {};
      dump(parse(input, ctx), out);
   }

   void dump(rush::source const& input, std::ostream& out) {
      ast::context ctx {};
      dump(parse(input, ctx), out);
   }

   void dump(std::istream& input, std::ostream& out) {
      ast::context ctx {};
      dump(parse(input, ctx), out);
   }

   void dump(std::string input, parser_options const& opts, std::ostream& out) {
      ast::context ctx {};
      dump(parse(input, ctx, opts), out);
   }

   void dump(std::istream& input, parser_options const& opts, std::ostream& out) {
      ast::context ctx {};
      dump(parse(input, ctx, opts), out);
   }

   void dump(rush::source const& input, parser_options const& opts, std::ostream& out) {
      ast::context ctx {};
      dump(parse(input, ctx, opts), out);
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
