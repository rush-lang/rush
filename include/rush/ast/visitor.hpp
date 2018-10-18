#pragma once

#ifndef RUSH_AST_VISITOR_HPP
#define RUSH_AST_VISITOR_HPP

namespace rush::ast {
	class type;
	class expression;
	class unary_expression;
	class binary_expression;
	class identifier_expression;

	class literal_expression;
	class nil_literal_expression;
	class string_literal_expression;
	class boolean_literal_expression;
	class integer_literal_expression;
	class floating_literal_expression;

	class statement;
	class statement_block;
	class if_statement;
	class for_statement;
	class case_statement;
	class switch_statement;
	class while_statement;
	class return_statement;

	class declaration;
	class constant_declaration;
	class variable_declaration;
	class function_declaration;

	class visitor {
	public:
		virtual ~visitor() = default;

		// types
		virtual void visit_type(ast::type const&) {};

		// declarations
		virtual void visit_constant_decl(constant_declaration const&) {};
		virtual void visit_variable_decl(variable_declaration const&) {};
		virtual void visit_function_decl(function_declaration const&) {};

		// statements
		virtual void visit_block_stmt(statement_block const&) {};

		virtual void visit_if_stmt(if_statement const&) {};
		virtual void visit_for_stmt(for_statement const&) {};
		virtual void visit_while_stmt(while_statement const&) {};
		virtual void visit_switch_stmt(switch_statement const&) {};

		virtual void visit_return_stmt(return_statement const&) {};

		// expressions
		virtual void visit_unary_expr(unary_expression const&) {};
		virtual void visit_binary_expr(binary_expression const&) {};
		virtual void visit_identifier_expr(identifier_expression const&) {};

		virtual void visit_literal_expr(nil_literal_expression const&) {}
		virtual void visit_literal_expr(string_literal_expression const&) {}
		virtual void visit_literal_expr(boolean_literal_expression const&) {}
		virtual void visit_literal_expr(integer_literal_expression const&) {}
		virtual void visit_literal_expr(floating_literal_expression const&) {}

#		define RUSH_VISITOR_FUNC_PROTOTYPES
#		include "rush/ast/_operators.hpp"
	};
} // rush

#endif // RUSH_AST_VISITOR_HPP
