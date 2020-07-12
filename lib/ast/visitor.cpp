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
#include "rush/ast/visitor.hpp"

#include "rush/ast/module.hpp"
#include "rush/ast/source.hpp"
#include "rush/ast/types.hpp"
#include "rush/ast/patterns.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"

namespace rush::ast {
   void visitor::visit_module(ast::module_node const& mdl) {
      std::for_each(mdl.begin(), mdl.end(),
         [this](auto& p) { p.accept(*this); });
   }

   void visitor::visit_source(ast::source_node const& src) {
      std::for_each(src.begin(), src.end(),
         [this](auto& p) { p.accept(*this); });
   }

   void visitor::visit_ptrn_list(ast::pattern_list const& ptrn) {
      std::for_each(ptrn.begin(), ptrn.end(),
         [this](auto& p) { p.accept(*this); });
   }

   void visitor::visit_expr_list(ast::expression_list const& expr) {
      std::for_each(expr.begin(), expr.end(),
         [this](auto& e) { e.accept(*this); });
   }

   void visitor::visit_throw_stmt(result_statement const& stmt) {
      visit_throw_stmt(static_cast<simple_statement const&>(stmt));
   }

   void visitor::visit_return_stmt(result_statement const& stmt) {
      visit_return_stmt(static_cast<simple_statement const&>(stmt));
   };

   void visitor::visit_iteration_stmt(iteration_statement const& stmt) {
      switch (stmt.kind()) {
      default: break; // assert! should be unreachable.
      case statement_kind::for_: visit_for_stmt(stmt); break;
      }
   }

   void visitor::visit_conditional_stmt(conditional_statement const& stmt) {
      switch (stmt.kind()) {
      default: break; // assert! should be unreachable.
      case statement_kind::if_: visit_if_stmt(stmt); break;
      case statement_kind::while_: visit_while_stmt(stmt); break;
      }
   }

   void visitor::visit_alternating_stmt(alternating_statement const& stmt) {
      switch (stmt.kind()) {
      default: break; // assert! should be unreachable.
      case statement_kind::else_: visit_else_stmt(stmt);
      }
   }

   void visitor::visit_simple_stmt(simple_statement const& stmt) {
#     define RUSH_SIMPLE_STMT_VISIT_SWITCH
#     include "rush/ast/stmts/_statements.hpp"
   };

   void visitor::visit_result_stmt(result_statement const& stmt) {
#     define RUSH_RESULT_STMT_VISIT_SWITCH
#     include "rush/ast/stmts/_statements.hpp"
   };

   void visitor::visit_unary_expr(unary_expression const& expr) {
#     define RUSH_UNARY_EXPRESSION_VISIT_SWITCH
#     include "rush/ast/exprs/_operators.hpp"
   }

   void visitor::visit_binary_expr(binary_expression const& expr) {
#		define RUSH_BINARY_EXPRESSION_VISIT_SWITCH
#		include "rush/ast/exprs/_operators.hpp"
   }
} // rush::ast
