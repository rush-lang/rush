#pragma once

#ifndef RUSH_AST_VISITOR_HPP
#define RUSH_AST_VISITOR_HPP

namespace rush::ast {
	class type;
	class expression;
	class unary_expression;
	class binary_expression;
	class literal_expression;
	class identifier_expression;

	class constant_declaration;
	class variable_declaration;

	class visitor {
	public:
		virtual ~visitor() = default;

		virtual void visit_type(ast::type const&) = 0;
		virtual void visit_unary_expr(unary_expression const&) = 0;
		virtual void visit_binary_expr(binary_expression const&) = 0;
		virtual void visit_literal_expr(literal_expression const&) = 0;
		virtual void visit_identifier_expr(identifier_expression const&) = 0;

		virtual void visit_constant_decl(constant_declaration const&) = 0;
		virtual void visit_variable_decl(variable_declaration const&) = 0;
	};

	// todo: generate this via injen instead.
	class descending_visitor : private visitor {
		virtual void visit_type(ast::type const&) override;
		virtual void visit_unary_expr(unary_expression const&) override;
		virtual void visit_binary_expr(binary_expression const&) override;
		virtual void visit_literal_expr(literal_expression const&) override;
		virtual void visit_identifier_expr(identifier_expression const&) override;

		virtual void visit_constant_decl(constant_declaration const&) override;
		virtual void visit_variable_decl(variable_declaration const&) override;

	public:
		virtual void visit_addition(binary_expression const&);
		virtual void visit_subtraction(binary_expression const&);
		virtual void visit_multiplication(binary_expression const&);
		virtual void visit_division(binary_expression const&);
		virtual void visit_modulo(binary_expression const&);
	};
} // rush

#endif // RUSH_AST_VISITOR_HPP
