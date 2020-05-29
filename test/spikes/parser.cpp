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
#include "rush/lexer/lex.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"
#include "rush/ast/ast.hpp"
#include "rush/ast/iterator.hpp"
#include <iostream>

char const* src = R"(
import std.io;

func main(args: string[]):
   println("hello world!");
   return 0;
)";

namespace ast = rush::ast;
namespace exprs = rush::ast::exprs;
namespace decls = rush::ast::decls;
namespace stmts = rush::ast::stmts;
namespace types = rush::ast::types;
namespace ptrns = rush::ast::ptrns;

class linear_printer : public ast::visitor {
public:
   virtual void visit_ptrn_list(ast::pattern_list const& ptrn) override {}
   virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override {
      std::cout << ptrn.name() << std::endl;
   }
   virtual void visit_nil_expr(ast::nil_expression const& expr) override {
      std::cout << "nil" << std::endl;
   }
   virtual void visit_binary_expr(ast::binary_expression const& expr) override {
      switch (expr.opkind()) {
      case ast::binary_operator::addition: std::cout << "addition" << std::endl; break;
      case ast::binary_operator::subtraction: std::cout << "subtraction" << std::endl; break;
      case ast::binary_operator::multiplication: std::cout << "multiplication" << std::endl; break;
      case ast::binary_operator::division: std::cout << "division" << std::endl; break;
      default: std::cout << "<unknown>" << std::endl; break;
      }
   }
};

int main() {
   std::vector<std::unique_ptr<ast::pattern>> ps;

   ps.push_back(ptrns::destructure_object(ptrns::name("x")));
   ps.push_back(ptrns::name("y"));
   auto l1 = ptrns::list(std::move(ps));

   ps.clear();
   ps.push_back(ptrns::destructure_object(std::move(l1)));
   ps.push_back(ptrns::name("z"));
   ps.push_back(ptrns::destructure_object(ptrns::name("w")));
   auto l2 = ptrns::list(std::move(ps));


   auto c = ast::context {};
   auto p = decls::constant(std::move(l2));
   p->attach(c);

   auto range = ast::iterator_range(p.get());

   auto b = range.end() == range.end();

   std::for_each(range.begin(), range.end(), [](auto& n) {
      rush::visit(n, linear_printer {});
   });

	// std::cout << src << std::endl;
	// std::cout << "--------------------" << std::endl;
	// rush::dump(src);
}
