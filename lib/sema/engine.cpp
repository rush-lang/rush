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
#include "rush/sema/engine.hpp"
#include "rush/ast/traversal.hpp"

#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/patterns.hpp"

using namespace rush;

class semantic_analysis_visitor : public ast::traversal {
public:
   semantic_analysis_visitor(sema::engine& eng)
      : _engine { eng } {}

   std::vector<std::unique_ptr<rush::diagnostic>> result() {
      return {};
      // return { nullptr, {} };
   }

   virtual void visit_constant_decl(ast::constant_declaration const& decl) override {

   }

   virtual void visit_variable_decl(ast::variable_declaration const& decl) override {

   }

   virtual void visit_function_decl(ast::function_declaration const& decl) override {

   }

   virtual void visit_class_decl(ast::class_declaration const& decl) override {

   }

   virtual void visit_struct_decl(ast::struct_declaration const& decl) override {

   }

   virtual void visit_member_access_expr(ast::member_access_expression const& expr) override {

   }

private:
   sema::engine& _engine;
};

namespace rush::sema {
   rush::semantic_analysis engine::analyze(rush::syntax_analysis const& ast) {
      auto v = semantic_analysis_visitor { *this };
      return { nullptr, rush::visit(ast, v).result() };
   }
}
