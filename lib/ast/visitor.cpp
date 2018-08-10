#include "rush/ast/visitor.hpp"

#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"

namespace rush::ast {
	void descending_visitor::visit_type(ast::type const& type) {
		// do nothing.
	}

	void descending_visitor::visit_unary_expr(unary_expression const& expr) {
		expr.operand().accept(*this);
		expr.result_type().accept(*this);
	}

	void descending_visitor::visit_binary_expr(binary_expression const& expr) {
		switch (expr.opkind()) {
		case binary_operator::modulo: this->visit_modulo(expr); break;
		case binary_operator::division: this->visit_division(expr); break;
		case binary_operator::addition: this->visit_addition(expr); break;
		case binary_operator::subtraction: this->visit_subtraction(expr); break;
		case binary_operator::multiplication: this->visit_multiplication(expr); break;
		}
		expr.result_type().accept(*this);
	}

	void descending_visitor::visit_literal_expr(literal_expression const& expr) {
		expr.result_type().accept(*this);
	}

	void descending_visitor::visit_identifier_expr(identifier_expression const& expr) {
		expr.result_type().accept(*this);
	}

	void descending_visitor::visit_constant_decl(constant_declaration const& expr) {
		expr.type().accept(*this);
		expr.initializer().accept(*this);
	}

	void descending_visitor::visit_variable_decl(variable_declaration const& expr) {
		expr.type().accept(*this);
		expr.initializer().accept(*this);
	}

	void descending_visitor::visit_addition(binary_expression const& expr) {
		expr.left_operand().accept(*this);
		expr.right_operand().accept(*this);
	}

	void descending_visitor::visit_subtraction(binary_expression const& expr) {
		expr.left_operand().accept(*this);
		expr.right_operand().accept(*this);
	}

	void descending_visitor::visit_multiplication(binary_expression const& expr) {
		expr.left_operand().accept(*this);
		expr.right_operand().accept(*this);
	}

	void descending_visitor::visit_division(binary_expression const& expr) {
		expr.left_operand().accept(*this);
		expr.right_operand().accept(*this);
	}

	void descending_visitor::visit_modulo(binary_expression const& expr) {
		expr.left_operand().accept(*this);
		expr.right_operand().accept(*this);
	}
} // rush::ast