#pragma once

#ifndef RUSH_AST_VISITOR_HPP
#define RUSH_AST_VISITOR_HPP

namespace rush::ast {
	class expression;
	class unary_expression;
	class binary_expression;
	class literal_expression;
	class variable_expression;

	class visitor {
	public:
		virtual ~visitor() = default;

		virtual void unary_expr(unary_expression const&) = 0;
		virtual void binary_expr(binary_expression const&) = 0;
		virtual void literal_expr(literal_expression const&) = 0;
		virtual void variable_expr(variable_expression const&) = 0;
	};

	// todo: generate this via injen instead.
	class descending_visitor : private visitor {
		virtual void unary_expr(unary_expression const&) override;
		virtual void binary_expr(binary_expression const&) override;
		virtual void literal_expr(literal_expression const&) override;
		virtual void variable_expr(variable_expression const&) override;

	public:
		virtual void addition_expr(binary_expression const&);
		virtual void subtraction_expr(binary_expression const&);
		virtual void multiplication_expr(binary_expression const&);
		virtual void division_expr(binary_expression const&);
	};
} // rush

#endif // RUSH_AST_VISITOR_HPP
