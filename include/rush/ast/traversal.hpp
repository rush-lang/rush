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
#pragma once

#ifndef RUSH_AST_TRAVERSAL_HPP
#define RUSH_AST_TRAVERSAL_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/module.hpp"
#include "rush/ast/source.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/patterns.hpp"

namespace rush::ast {
   /*! \brief A traversal derives the AST visitor, in which relevant visitation
    *         methods are overloaded to visit an AST in its entirety. */
   class traversal : public ast::visitor {
   public:

      virtual void visit_module(ast::module_node const& mdl) override {
         std::for_each(mdl.begin(), mdl.end(), [this](auto& n) { accept(n); });
      }

      virtual void visit_source(ast::source_node const& src) override {
         std::for_each(src.begin(), src.end(), [this](auto& n) { accept(n); });
      }

      virtual void visit_binding_ptrn(ast::binding_pattern const& ptrn) override {
         accept(ptrn.pattern());
         accept(ptrn.expression());
      }

      virtual void visit_array_destructure_ptrn(ast::array_destructure_pattern const& ptrn) override {
         accept(ptrn.pattern());
      }

      virtual void visit_object_destructure_ptrn(ast::object_destructure_pattern const& ptrn) override {
         accept(ptrn.pattern());
      }

      virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const& ptrn) override {
         accept(ptrn.pattern());
      }

      virtual void visit_rest_ptrn(ast::rest_pattern const& ptrn) override {
         accept(ptrn.pattern());
      }

		// declarations
      virtual void visit_module_decl(ast::module_declaration const& decl) override {
         accept(decl.declaration());
      }

      virtual void visit_extern_decl(ast::extern_declaration const& decl) override {
         accept(decl.declaration());
      }

      virtual void visit_base_decl(ast::base_declaration const& decl) override {
         accept(decl.declaration());
      }

      virtual void visit_async_decl(ast::async_declaration const& decl) override {
         accept(decl.declaration());
      }

      virtual void visit_modified_decl(ast::modified_declaration const& decl) override {
         accept(decl.declaration());
      }

      virtual void visit_storage_decl(ast::storage_declaration const& decl) override {
         accept(decl.pattern());
      }

		virtual void visit_function_decl(ast::function_declaration const& decl) override {
         accept(decl.parameters());
         accept(decl.body());
      }

      virtual void visit_class_decl(ast::class_declaration const& decl) override {
         auto sections = decl.sections();
         std::for_each(sections.begin(), sections.end(),
            [this](auto& s) { accept(s); });
      }

      virtual void visit_struct_decl(ast::struct_declaration const& decl) override {
         auto sections = decl.sections();
         std::for_each(sections.begin(), sections.end(),
            [this](auto& s) { accept(s); });
      }

      virtual void visit_member_section_decl(ast::member_section_declaration const& decl) override {
         auto members = decl.members();
         std::for_each(members.begin(), members.end(),
            [this](auto& m) { accept(m); });
      }

      virtual void visit_field_decl(ast::field_declaration const& decl) override {
         accept(decl.pattern());
      }

      virtual void visit_property_getter_decl(ast::property_getter_declaration const& decl) override {
         accept(decl.body());
      }

      virtual void visit_property_setter_decl(ast::property_setter_declaration const& decl) override {
         accept(decl.body());
      }

      virtual void visit_method_decl(ast::method_declaration const& decl) override {
         accept(decl.parameters());
         accept(decl.body());
      }


		// statements
		virtual void visit_block_stmt(ast::statement_block const& stmt) override {
         std::for_each(stmt.begin(), stmt.end(),
            [this](auto& s) { accept(s); });
      }

		virtual void visit_switch_stmt(ast::switch_statement const& stmt) override {
         // todo: implement when switch statements are implemented.
      }

      virtual void visit_ternary_expr(ast::ternary_expression const& expr) override {
         accept(expr.condition());
         accept(expr.true_expr());
         accept(expr.false_expr());
      }

      virtual void visit_member_access_expr(ast::member_access_expression const& expr) override {
         accept(expr.expression());
         accept(expr.member());
      }

      virtual void visit_string_template_expr(string_template_expression const& expr) override {
         auto parts = expr.parts();
         std::for_each(parts.begin(), parts.end(),
            [this](auto& p) { accept(p); });
      }

      virtual void visit_invoke_expr(ast::invoke_expression const& expr) override {
         accept(expr.callable());
         accept(expr.arguments());
      }

      virtual void visit_lambda_expr(ast::lambda_expression const& expr) override {
         accept(expr.parameters());
         accept(expr.body());
      }

      virtual void visit_literal_expr(ast::array_literal_expression const& expr) override {
         accept(expr.elements());
      }

      virtual void visit_literal_expr(ast::tuple_literal_expression const& expr) override {
         accept(expr.arguments());
      }

      virtual void visit_new_expr(ast::new_expression const& expr) override {
         accept(expr.expression());
      }

      virtual void visit_spread_expr(ast::spread_expression const& expr) override {
         accept(expr.expression());
      }

      virtual void visit_parenthesis_expr(parenthesis_expression const& expr) override {
         accept(expr.expression());
      }

      virtual void visit_subscript_expr(subscript_expression const& expr) override {
         accept(expr.operand());
         accept(expr.subscript());
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

      void traverse_iteration_stmt(ast::iteration_statement const& stmt) {
         accept(stmt.pattern());
         accept(stmt.expression());
         accept(stmt.body());
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
