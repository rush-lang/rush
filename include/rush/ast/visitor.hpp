#pragma once

#ifndef RUSH_AST_VISITOR_HPP
#define RUSH_AST_VISITOR_HPP

#include <type_traits>
#include <utility>

namespace rush::ast {
   class type;
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

	class expression;
   class argument;
   class named_argument;
	class unary_expression;
	class binary_expression;
   class ternary_expression;
	class identifier_expression;
   class member_access_expression;
   class string_template_expression;
   class invoke_expression;
   class lambda_expression;

	class literal_expression;
	class nil_literal_expression;
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
   class parameter;
	class declaration;
   class import_declaration;
   class module_declaration;
	class constant_declaration;
	class variable_declaration;
	class function_declaration;
   class member_declaration;
   class class_declaration;

	class visitor {
	public:
		virtual ~visitor() = default;

		// types
		virtual void visit_type(ast::type const&) {};
      virtual void visit_builtin_error_type(ast::builtin_error_type const&) {}
      virtual void visit_builtin_void_type(ast::builtin_void_type const&) {}
      virtual void visit_builtin_bool_type(ast::builtin_bool_type const&) {}
      virtual void visit_builtin_integral_type(ast::builtin_integral_type const&) {}
      virtual void visit_builtin_floating_type(ast::builtin_floating_point_type const&) {}
      virtual void visit_builtin_string_type(ast::builtin_string_type const&) {}

      virtual void visit_array_type(ast::array_type const&) {};
      virtual void visit_tuple_type(ast::tuple_type const&) {};
      virtual void visit_enum_type(ast::enum_type const&) {};
      virtual void visit_class_type(ast::class_type const&) {};
      virtual void visit_struct_type(ast::struct_type const&) {};
      virtual void visit_concept_type(ast::concept_type const&) {};
      virtual void visit_interface_type(ast::interface_type const&) {};
      virtual void visit_function_type(ast::function_type const&) {};

		// declarations
      virtual void visit_module(ast::module const&) {};
      virtual void visit_import_decl(ast::import_declaration const&) {};
      virtual void visit_module_decl(ast::module_declaration const&) {};

      virtual void visit_parameter_decl(ast::parameter const&) {};
		virtual void visit_constant_decl(ast::constant_declaration const&) {};
		virtual void visit_variable_decl(ast::variable_declaration const&) {};
		virtual void visit_function_decl(ast::function_declaration const&) {};
      virtual void visit_member_decl(ast::member_declaration const&) {};
      virtual void visit_class_decl(ast::class_declaration const&) {}

		// statements
		virtual void visit_block_stmt(statement_block const&) {};

      virtual void visit_iteration_stmt(iteration_statement const&) {}
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
      virtual void visit_argument(argument const&) {};
      virtual void visit_named_argument(named_argument const&) {};

		virtual void visit_unary_expr(unary_expression const&);
		virtual void visit_binary_expr(binary_expression const&);
      virtual void visit_ternary_expr(ternary_expression const&) {};
		virtual void visit_identifier_expr(identifier_expression const&) {};
      virtual void visit_member_access_expr(member_access_expression const&) {};
      virtual void visit_string_template_expr(string_template_expression const&) {};
      virtual void visit_invoke_expr(invoke_expression const&) {};
      virtual void visit_lambda_expr(lambda_expression const&) {};

		virtual void visit_literal_expr(nil_literal_expression const&) {};
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
