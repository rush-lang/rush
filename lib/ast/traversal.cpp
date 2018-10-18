#include "rush/ast/traversal.hpp"
#include "rush/ast/expression.hpp"
#include "rush/ast/statement.hpp"
#include "rush/ast/declaration.hpp"

namespace rush::ast {
	// void traversal::visit_type(ast::type const&) {}

	void traversal::visit_constant_decl(constant_declaration const& decl) {
		decl.initializer().accept(*this);
		decl.type().accept(*this);
	}

	void traversal::visit_variable_decl(variable_declaration const& decl) {
		decl.initializer().accept(*this);
		decl.type().accept(*this);
	}

	void traversal::visit_block_stmt(statement_block const&) {}
	void traversal::visit_if_stmt(if_statement const&) {}
	void traversal::visit_for_stmt(for_statement const&) {}
	void traversal::visit_switch_stmt(switch_statement const&) {}
	void traversal::visit_while_stmt(while_statement const&) {}
	void traversal::visit_return_stmt(return_statement const&) {}

	void traversal::visit_unary_expr(unary_expression const& expr) {
		expr.operand().accept(*this);
		expr.result_type().accept(*this);
	}

	void traversal::visit_binary_expr(binary_expression const& expr) {
		expr.left_operand().accept(*this);
		expr.right_operand().accept(*this);
		expr.result_type().accept(*this);
	}

	// void traversal::visit_identifier_expr(identifier_expression const&) {}
	// void traversal::visit_literal_expr(nil_literal_expression const&) {}
	// void traversal::visit_literal_expr(string_literal_expression const&) {}
	// void traversal::visit_literal_expr(boolean_literal_expression const&) {}
	// void traversal::visit_literal_expr(integer_literal_expression const&) {}
	// void traversal::visit_literal_expr(floating_literal_expression const&) {}
}