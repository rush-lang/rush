#include "rush/ast/expr/identifier.hpp"
#include "rush/ast/expr/literal.hpp"
#include "rush/ast/expr/binary.hpp"
#include "rush/ast/expr/unary.hpp"

#include "parser.hpp"

namespace rush {

#define RUSH_IS_UNARY_OP_FUNC
#define RUSH_IS_BINARY_OP_FUNC
#define RUSH_PRECEDENCE_FUNC
#include "rush/ast/_operators.hpp"

	int compare_binary_op_precedence(lexical_token const& lhs, lexical_token const& rhs) {
		return is_binary_op(lhs) && is_binary_op(rhs)
			? binary_precedence(lhs) - binary_precedence(rhs) : 0;
	}

	std::unique_ptr<ast::expression> parser::parse_expression() {
		auto expr = parse_primary_expression();
		if (!expr) return nullptr;

		if (is_binary_op(peek_skip_indent()))
			return parse_binary_expression(std::move(expr));

		return std::move(expr);
	}

	std::unique_ptr<ast::expression> parser::parse_parenthesised_expression() {
		assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected token to be \'(\'");
		next_skip_indent(); // consume '('
		auto expr = parse_expression();
		if (!expr) return nullptr;

		auto tok = peek_skip_indent();
		if (tok.is(symbols::right_parenthesis)) {
			next_skip_indent(); // consume ')'
			return expr;
		}

		return error("expected a closing \')\' before \'{}\'", tok);
	}

	std::unique_ptr<ast::expression> parser::parse_primary_expression() {
		auto tok = peek_skip_indent();
		switch (tok.type()) {
		default: return error("expected primary expression, but found '{}'", tok);
		case lexical_token_type::error: return error(tok.text());
		case lexical_token_type::identifier: return parse_identifier_expression();
		case lexical_token_type::string_literal: return parse_string_expression();
		case lexical_token_type::integer_literal: return parse_integer_expression();
		case lexical_token_type::floating_literal: return parse_floating_expression();
		case lexical_token_type::keyword:
			if (tok.is(keywords::true_)) return ast::literal_expr(true);
			if (tok.is(keywords::false_)) return ast::literal_expr(false);
			return error("unexpected keyword '{}' when parsing primary expression", tok);
		case lexical_token_type::symbol:
			switch (tok.symbol()) {
			default: return error("unexpected symbol '{}' in expression", tok);
			case symbols::left_parenthesis: return parse_parenthesised_expression();
			}
		}
	}

	std::unique_ptr<ast::expression> parser::parse_string_expression() {
		assert(peek_skip_indent().is_string_literal() && "expected token to be a string literal.");
		auto tok = next_skip_indent();
		auto str = tok.text();
		return ast::literal_expr(str);
	}

	std::unique_ptr<ast::expression> parser::parse_integer_expression() {
		assert(peek_skip_indent().is_integer_literal() && "expected token to be an integer literal.");
		auto tok = next_skip_indent();
		auto val = tok.integer_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return ast::literal_expr(val, ast::int_type);
		case lexical_token_suffix::long_literal: return ast::literal_expr(val, ast::long_type);
		case lexical_token_suffix::unsigned_literal: return ast::literal_expr(val, ast::uint_type);
		// case lexical_token_suffix::unsigned_long_literal: return ast::literal_expr(tok.integer(), ast::ulong_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_floating_expression() {
		assert(peek_skip_indent().is_floating_literal() && "expected token to be a floating literal.");
		auto tok = next_skip_indent();
		auto val = tok.floating_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return ast::literal_expr(val, ast::double_type);
		case lexical_token_suffix::float_literal: return ast::literal_expr(val, ast::float_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_identifier_expression() {
		assert(peek_skip_indent().is_identifier() && "expected token to be an identifier.");
		auto tok = next_skip_indent();
		auto ident = tok.text();
		return ast::identifier_expr(_scope, ident);
	}

	std::unique_ptr<ast::binary_expression> parser::parse_binary_expression(std::unique_ptr<ast::expression> lhs) {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto tok = peek_skip_indent();
		std::unique_ptr<ast::binary_expression> expr;
		switch (tok.symbol()) {
		default: return error("unexpected symbol '{}'", tok);
		case symbols::plus: expr = parse_addition_expression(std::move(lhs)); break;
		case symbols::minus: expr = parse_subtraction_expression(std::move(lhs)); break;
		case symbols::asterisk: expr = parse_multiplication_expression(std::move(lhs)); break;
		case symbols::forward_slash: expr = parse_division_expression(std::move(lhs)); break;
		}

		return is_binary_op(peek_skip_indent())
			? parse_binary_expression(std::move(expr))
			: std::move(expr);
	}

	std::unique_ptr<ast::expression> parser::parse_binary_expression_rhs() {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto prev = next_skip_indent(); // consume binary operator symbol.
		auto rhs = parse_primary_expression();
		auto curr = peek_skip_indent(); // look-ahead for possible binary operator.

		if (compare_binary_op_precedence(curr, prev) < 0)
			rhs = parse_binary_expression(std::move(rhs));

		return std::move(rhs);
	}

	std::unique_ptr<ast::binary_expression> parser::parse_addition_expression(std::unique_ptr<ast::expression> lhs) {
		assert(peek_skip_indent().is(symbols::plus) && "expected token to be an addition symbol.");
		return ast::addition_expr(std::move(lhs), parse_binary_expression_rhs());
	}

	std::unique_ptr<ast::binary_expression> parser::parse_subtraction_expression(std::unique_ptr<ast::expression> lhs) {
		assert(peek_skip_indent().is(symbols::minus) && "expected token to be a subtraction symbol.");
		return ast::subtraction_expr(std::move(lhs), parse_binary_expression_rhs());
	}

	std::unique_ptr<ast::binary_expression> parser::parse_multiplication_expression(std::unique_ptr<ast::expression> lhs) {
		assert(peek_skip_indent().is(symbols::asterisk) && "expected token to be a multiplication symbol.");
		return ast::multiplication_expr(std::move(lhs), parse_binary_expression_rhs());
	}

	std::unique_ptr<ast::binary_expression> parser::parse_division_expression(std::unique_ptr<ast::expression> lhs) {
		assert(peek_skip_indent().is(symbols::forward_slash) && "expected token to be a division symbol.");
		return ast::division_expr(std::move(lhs), parse_binary_expression_rhs());
	}
}