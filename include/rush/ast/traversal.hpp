#pragma once

#ifndef RUSH_AST_TRAVERSAL_HPP
#define RUSH_AST_TRAVERSAL_HPP

#include "rush/ast/visitor.hpp"

namespace rush::ast {
	// A travesal derives the AST visitor, in which relevant visitation
	// methods are overloaded to visit the AST in its entirety.
	class traversal : public visitor {
	public:
		// types
		// virtual void visit_type(ast::type const&) override;

		// declarations
		virtual void visit_constant_decl(constant_declaration const&) override;
		virtual void visit_variable_decl(variable_declaration const&) override;

		// statements
		virtual void visit_block_stmt(statement_block const&) override;
		virtual void visit_if_stmt(if_statement const&) override;
		virtual void visit_for_stmt(for_statement const&) override;
		virtual void visit_switch_stmt(switch_statement const&) override;
		virtual void visit_while_stmt(while_statement const&) override;
		virtual void visit_return_stmt(return_statement const&) override;

		// expressions
		virtual void visit_unary_expr(unary_expression const&) override;
		virtual void visit_binary_expr(binary_expression const&) override;
		// virtual void visit_identifier_expr(identifier_expression const&) override;

		// virtual void visit_literal_expr(nil_literal_expression const&) override;
		// virtual void visit_literal_expr(string_literal_expression const&) override;
		// virtual void visit_literal_expr(boolean_literal_expression const&) override;
		// virtual void visit_literal_expr(integer_literal_expression const&) override;
		// virtual void visit_literal_expr(floating_literal_expression const&) override;
	};
} // rush::ast

#endif // RUSH_AST_TRAVERSAL_HPP
