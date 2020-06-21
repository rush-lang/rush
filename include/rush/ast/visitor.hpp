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

#ifndef RUSH_AST_VISITOR_HPP
#define RUSH_AST_VISITOR_HPP

#include <type_traits>
#include <utility>

namespace rush::ast {
   class type;
   class named_type;
   class type_extension;
   class builtin_error_type;
   class builtin_void_type;
   class builtin_bool_type;
   class builtin_char_type;
   class builtin_string_type;
   class builtin_integral_type;
   class builtin_floating_point_type;
   class array_type;
   class tuple_type;
   class enum_type;
   class class_type;
   class struct_type;
   class concept_type;
   class interface_type;
   class function_type;
   class optional_type;
   class user_type;

   class pattern_list;
   class named_pattern;
   class discard_pattern;
   class binding_pattern;
   class array_destructure_pattern;
   class object_destructure_pattern;
   class type_annotation_pattern;

	class expression;
   class expression_list;
	class unary_expression;
	class binary_expression;
   class ternary_expression;
	class identifier_expression;
   class member_access_expression;
   class string_template_expression;
   class invoke_expression;
   class lambda_expression;
	class nil_expression;
   class new_expression;
   class this_expression;
   class base_expression;

	class literal_expression;
   class array_literal_expression;
   class tuple_literal_expression;
	class string_literal_expression;
	class boolean_literal_expression;
	class integer_literal_expression;
	class floating_literal_expression;

	class statement;
	class statement_block;
	class case_statement;
	class switch_statement;
   class iteration_statement;
   class conditional_statement;
   class alternating_statement;

   class simple_statement;
   class result_statement;

   class module;
   class import_declaration;
   class module_declaration;

	class constant_declaration;
	class variable_declaration;
	class function_declaration;
   class parameter_declaration;

   class class_declaration;
   class struct_declaration;
   class member_section_declaration;
   class variable_field_declaration;
   class constant_field_declaration;
   class property_getter_declaration;
   class property_setter_declaration;
   class method_declaration;

	class visitor {
	public:
		virtual ~visitor() = default;

		// types
		virtual void visit_type(ast::type const&) {};
      virtual void visit_named_type(ast::named_type const&) {};
      virtual void visit_type_extension(ast::type_extension const&) {};
      virtual void visit_builtin_error_type(ast::builtin_error_type const&) {};
      virtual void visit_builtin_void_type(ast::builtin_void_type const&) {};
      virtual void visit_builtin_bool_type(ast::builtin_bool_type const&) {};
      virtual void visit_builtin_integral_type(ast::builtin_integral_type const&) {};
      virtual void visit_builtin_floating_type(ast::builtin_floating_point_type const&) {};
      virtual void visit_builtin_string_type(ast::builtin_string_type const&) {};

      virtual void visit_array_type(ast::array_type const&) {};
      virtual void visit_tuple_type(ast::tuple_type const&) {};
      virtual void visit_enum_type(ast::enum_type const&) {};
      virtual void visit_class_type(ast::class_type const&) {};
      virtual void visit_struct_type(ast::struct_type const&) {};
      virtual void visit_concept_type(ast::concept_type const&) {};
      virtual void visit_interface_type(ast::interface_type const&) {};
      virtual void visit_function_type(ast::function_type const&) {};
      virtual void visit_optional_type(ast::optional_type const&) {};
      virtual void visit_user_type(ast::user_type const&) {};

      // patterns
      virtual void visit_ptrn_list(ast::pattern_list const&);
      virtual void visit_named_ptrn(ast::named_pattern const&) {};
      virtual void visit_discard_ptrn(ast::discard_pattern const&) {};
      virtual void visit_binding_ptrn(ast::binding_pattern const&) {};
      virtual void visit_array_destructure_ptrn(ast::array_destructure_pattern const&) {};
      virtual void visit_object_destructure_ptrn(ast::object_destructure_pattern const&) {};
      virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const&) {};

		// declarations
      virtual void visit_module(ast::module const&) {};
      virtual void visit_import_decl(ast::import_declaration const&) {};
      virtual void visit_module_decl(ast::module_declaration const&) {};

		virtual void visit_constant_decl(ast::constant_declaration const&) {};
		virtual void visit_variable_decl(ast::variable_declaration const&) {};

      virtual void visit_parameter_decl(ast::parameter_declaration const&) {};
		virtual void visit_function_decl(ast::function_declaration const&) {};

      virtual void visit_class_decl(ast::class_declaration const&) {};
      virtual void visit_struct_decl(ast::struct_declaration const&) {};
      virtual void visit_member_section_decl(ast::member_section_declaration const&) {};

      virtual void visit_variable_field_decl(ast::variable_field_declaration const&) {};
      virtual void visit_constant_field_decl(ast::constant_field_declaration const&) {};
      virtual void visit_property_getter_decl(ast::property_getter_declaration const&) {};
      virtual void visit_property_setter_decl(ast::property_setter_declaration const&) {};
      virtual void visit_method_decl(ast::method_declaration const&) {};

		// statements
		virtual void visit_block_stmt(statement_block const&) {};

      virtual void visit_iteration_stmt(iteration_statement const&);
      virtual void visit_conditional_stmt(conditional_statement const&);
      virtual void visit_alternating_stmt(alternating_statement const&);

		virtual void visit_if_stmt(conditional_statement const&) {};
      virtual void visit_else_stmt(alternating_statement const&) {};
		virtual void visit_for_stmt(iteration_statement const&) {};
		virtual void visit_while_stmt(conditional_statement const&) {};
		virtual void visit_switch_stmt(switch_statement const&) {};

#     define RUSH_VISITOR_SIMPLE_STMT_FUNC_PROTOTYPES
#     include "rush/ast/stmts/_statements.hpp"

      virtual void visit_simple_stmt(simple_statement const& stmt);
      virtual void visit_result_stmt(result_statement const& stmt);

		// expressions
      virtual void visit_expr_list(expression_list const&);
		virtual void visit_unary_expr(unary_expression const&);
		virtual void visit_binary_expr(binary_expression const&);
      virtual void visit_ternary_expr(ternary_expression const&) {};
		virtual void visit_identifier_expr(identifier_expression const&) {};
      virtual void visit_member_access_expr(member_access_expression const&) {};
      virtual void visit_string_template_expr(string_template_expression const&) {};
      virtual void visit_invoke_expr(invoke_expression const&) {};
      virtual void visit_lambda_expr(lambda_expression const&) {};
		virtual void visit_nil_expr(nil_expression const&) {};
		virtual void visit_new_expr(new_expression const&) {};
		virtual void visit_this_expr(this_expression const&) {};
		virtual void visit_base_expr(base_expression const&) {};

		virtual void visit_literal_expr(string_literal_expression const&) {};
		virtual void visit_literal_expr(boolean_literal_expression const&) {};
		virtual void visit_literal_expr(integer_literal_expression const&) {};
		virtual void visit_literal_expr(floating_literal_expression const&) {};
      virtual void visit_literal_expr(array_literal_expression const&) {};
      virtual void visit_literal_expr(tuple_literal_expression const&) {};

#		define RUSH_VISITOR_FUNC_PROTOTYPES
#		include "rush/ast/exprs/_operators.hpp"
	};
} // rush::ast
#include <iostream>
namespace rush {
   template <typename NodeT, typename VisitorT>
   std::enable_if_t<
      std::is_base_of_v<ast::visitor, std::remove_reference_t<VisitorT>>,
      VisitorT&&> visit(NodeT const& n, VisitorT&& v) {
         n.accept(v);
         return std::forward<VisitorT>(v);
      }
} // rush

#endif // RUSH_AST_VISITOR_HPP
