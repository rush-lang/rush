#pragma once

#ifndef RUSH_AST_TRAVERSAL_HPP
#define RUSH_AST_TRAVERSAL_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/statements.hpp"

namespace rush::ast {
   /*! \brief A traversal derives the AST visitor, in which relevant visitation
    *         methods are overloaded to visit an AST in its entirety. */
   class traversal : public ast::visitor {
   public:
      virtual void visit_module(ast::module const& mdl) override {
         std::for_each(mdl.imports().begin(), mdl.imports().end(), [this](auto& decl) { accept(*decl); });
         std::for_each(mdl.declarations().begin(), mdl.declarations().end(), [this](auto& decl) { accept(*decl); });
      }

      virtual void visit_module_decl(ast::module_declaration const& exp) override {
         accept(exp.declaration());
      }

		// declarations
		virtual void visit_constant_decl(ast::constant_declaration const& decl) override {
         if (decl.initializer()) accept(*decl.initializer());
      }

		virtual void visit_variable_decl(ast::variable_declaration const& decl) override {
         if (decl.initializer()) accept(*decl.initializer());
      }

		virtual void visit_function_decl(ast::function_declaration const& decl) override {
         accept(decl.parameters());
         accept(decl.body());
      }

		// statements
		virtual void visit_block_stmt(ast::statement_block const& stmt) override {
         for (auto& s : stmt) accept(*s);
      }

		virtual void visit_switch_stmt(ast::switch_statement const& stmt) override {
         // todo: implement when switch statements are implemented.
      }

      virtual void visit_ternary_expr(ast::ternary_expression const& expr) override {
         accept(expr.condition());
         accept(expr.true_expr());
         accept(expr.false_expr());
      }

      virtual void visit_invocation_expr(ast::invocation_expression const& expr) override {
         accept(expr.callable());
         accept(expr.arguments());
      }

#     define RUSH_TRAVERSAL_RESULT_STMT_FUNC_IMPLS
#     define RUSH_TRAVERSAL_ITERATION_STMT_FUNC_IMPLS
#     define RUSH_TRAVERSAL_CONDITIONAL_STMT_FUNC_IMPLS
#     define RUSH_TRAVERSAL_ALTERNATING_STMT_FUNC_IMPLS
#     include "rush/ast/stmts/_statements.hpp"

#     define RUSH_TRAVERSAL_UNARY_EXPR_FUNC_IMPLS
#     define RUSH_TRAVERSAL_BINARY_EXPR_FUNC_IMPLS
#     include "rush/ast/exprs/_operators.hpp"

   protected:
      void traverse(ast::node const& ast);

      virtual void accept(ast::node const& ast) {
         ast.accept(*this);
      }

   private:
      ast::visitor* _vis;

      void traverse_result_stmt(ast::result_statement const& stmt) {
         accept(stmt.expression());
      }

      void traverse_conditional_stmt(ast::conditional_statement const& stmt) {
         accept(stmt.condition());
         accept(stmt.body());
      }

      void traverse_alternating_stmt(ast::alternating_statement const& stmt) {
         accept(stmt.primary());
         accept(stmt.alternate());
      }

      void traverse_unary_expr(unary_expression const& expr) {
         accept(expr.operand());
      }

      void traverse_binary_expr(binary_expression const& expr) {
         accept(expr.left_operand());
         accept(expr.right_operand());
      }
   };
} // rush::ast

#endif // RUSH_AST_TRAVERSAL_HPP
