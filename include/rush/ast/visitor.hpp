#pragma once

#ifndef RUSH_AST_VISITOR_HPP
#define RUSH_AST_VISITOR_HPP

namespace rush::ast {
	class type;
	class expression;
	class unary_expression;
	class binary_expression;
	class literal_expression;
	class string_literal_expression;
	class boolean_literal_expression;
	class integer_literal_expression;
	class floating_literal_expression;
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

		virtual void visit_literal_expr(string_literal_expression const&) {}
		virtual void visit_literal_expr(boolean_literal_expression const&) {}
		virtual void visit_literal_expr(integer_literal_expression const&) {}
		virtual void visit_literal_expr(floating_literal_expression const&) {}

#		define RUSH_VISITOR_FUNC_PROTOTYPES
#		include "rush/ast/_operators.hpp"
	};

	namespace detail {
		// class ascending_visitor;
		class descending_visitor;
	}

	// detail::ascending_visitor ascending(visitor&);
	// detail::ascending_visitor ascending(visitor const&);

	detail::descending_visitor descending_visitor(visitor&);
	detail::descending_visitor descending_visitor(visitor const&);
} // rush

#endif // RUSH_AST_VISITOR_HPP
