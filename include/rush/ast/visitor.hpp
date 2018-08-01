#pragma once

#ifndef RUSH_AST_VISITOR_HPP
#define RUSH_AST_VISITOR_HPP

namespace rush::ast {
	class type;
	class expression;
	class unary_expression;
	class binary_expression;
	class literal_expression;
	class variable_expression;

	class visitor {
	public:
		virtual ~visitor() = default;

		virtual void visit_type(ast::type const&) = 0;
		virtual void visit_unary_expr(unary_expression const&) = 0;
		virtual void visit_binary_expr(binary_expression const&) = 0;
		virtual void visit_literal_expr(literal_expression const&) = 0;
		virtual void visit_variable_expr(variable_expression const&) = 0;
	};

	// todo: generate this via injen instead.
	class descending_visitor : private visitor {
		virtual void visit_type(ast::type const&) override;
		virtual void visit_unary_expr(unary_expression const&) override;
		virtual void visit_binary_expr(binary_expression const&) override;
		virtual void visit_literal_expr(literal_expression const&) override;
		virtual void visit_variable_expr(variable_expression const&) override;

	public:
		virtual void visit_addition(binary_expression const&);
		virtual void visit_subtraction(binary_expression const&);
		virtual void visit_multiplication(binary_expression const&);
		virtual void visit_division(binary_expression const&);
	};
} // rush

#endif // RUSH_AST_VISITOR_HPP
