#pragma once

#ifndef RUSH_AST_TRAVERSAL_HPP
#define RUSH_AST_TRAVERSAL_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/statements.hpp"

namespace rush::ast {
   /*! \brief A traversal_visitor derives the AST visitor, in which relevant visitation
    *         methods are overloaded to visit an AST in its entirety. */
   class traversal : public ast::visitor {
   public:
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

		virtual void visit_if_stmt(ast::if_statement const& stmt) override {
         accept(stmt.condition());
         accept(stmt.then());
         if (stmt.else_())
            accept(*stmt.else_());
      }

		virtual void visit_for_stmt(ast::for_statement const& stmt) override {
         // todo: implement when for statements are implemented.
      }

		virtual void visit_while_stmt(ast::while_statement const& stmt) override {
         accept(stmt.condition());
         accept(stmt.body());
      }

		virtual void visit_switch_stmt(ast::switch_statement const& stmt) override {
         // todo: implement when switch statements are implemented.
      }

		virtual void visit_return_stmt(ast::result_statement const& stmt) override {
         accept(stmt.expression());
      }

		virtual void visit_yield_stmt(ast::result_statement const& stmt) override {
         accept(stmt.expression());
      }

		// expressions
		virtual void visit_unary_expr(ast::unary_expression const& expr) override {
         accept(expr.operand());
      }

		virtual void visit_binary_expr(ast::binary_expression const& expr) override {
         accept(expr.left_operand());
         accept(expr.right_operand());
      }

      virtual void visit_ternary_expr(ast::ternary_expression const& expr) override {
         accept(expr.condition());
         accept(expr.true_expr());
         accept(expr.false_expr());
      }

      virtual void visit_invocation_expr(ast::invocation_expression const& expr) override {
         accept(expr.arguments());
         accept(expr.callable());
      }

   protected:
      void traverse(ast::node const& ast);

      virtual void accept(ast::node const& ast) {
         ast.accept(*this);
      }

   private:
      ast::visitor* _vis;
   };
} // rush::ast

#endif // RUSH_AST_TRAVERSAL_HPP
