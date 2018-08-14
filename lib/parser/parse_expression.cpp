#include "rush/ast/expr/identifier.hpp"
#include "rush/ast/expr/literal.hpp"
#include "rush/ast/expr/binary.hpp"
#include "rush/ast/expr/unary.hpp"

#include "parser.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;

namespace rush {

#define RUSH_IS_UNARY_OP_FUNC
#define RUSH_IS_BINARY_OP_FUNC
#define RUSH_PRECEDENCE_FUNC
#include "rush/ast/_operators.hpp"

	int compare_binary_op_precedence(lexical_token const& lhs, lexical_token const& rhs) {
		return is_binary_op(lhs) && is_binary_op(rhs)
			? binary_precedence(lhs) - binary_precedence(rhs) : 0;
	}

	std::unique_ptr<ast::expression> parser::parse_expr() {
		auto expr = parse_primary_expr();
		if (!expr) return nullptr;

		if (is_binary_op(peek_skip_indent()))
			return parse_binary_expr(std::move(expr));

		return std::move(expr);
	}

	std::unique_ptr<ast::expression> parser::parse_paren_expr() {
		assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected token to be \'(\'");
		next_skip_indent(); // consume '('
		auto expr = parse_expr();
		if (!expr) return nullptr;

		auto tok = peek_skip_indent();
		if (tok.is(symbols::right_parenthesis)) {
			next_skip_indent(); // consume ')'
			return expr;
		}

		return error("expected a closing \')\' before \'{}\'", tok);
	}

	std::unique_ptr<ast::expression> parser::parse_primary_expr() {
		auto tok = peek_skip_indent();
		switch (tok.type()) {
		default: return error("expected primary expression, but found '{}'", tok);
		case lexical_token_type::error: return error("", tok);
		case lexical_token_type::identifier: return parse_identifier_expr();
		case lexical_token_type::string_literal: return parse_string_expr();
		case lexical_token_type::integer_literal: return parse_integer_expr();
		case lexical_token_type::floating_literal: return parse_floating_expr();
		case lexical_token_type::keyword:
			if (tok.is(keywords::true_)) return exprs::literal(true);
			if (tok.is(keywords::false_)) return exprs::literal(false);
			return error("unexpected keyword '{}' parsing primary expression", tok);
		case lexical_token_type::symbol:
			switch (tok.symbol()) {
			default: return error("unexpected symbol '{}' in expression", tok);
			case symbols::left_parenthesis: return parse_paren_expr();
			}
		}
	}

	std::unique_ptr<ast::expression> parser::parse_string_expr() {
		assert(peek_skip_indent().is_string_literal() && "expected token to be a string literal.");
		auto tok = next_skip_indent();
		auto str = tok.text();
		return exprs::literal(str);
	}

	std::unique_ptr<ast::expression> parser::parse_integer_expr() {
		assert(peek_skip_indent().is_integer_literal() && "expected token to be an integer literal.");
		auto tok = next_skip_indent();
		auto val = tok.integer_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return exprs::literal(val, ast::int_type);
		case lexical_token_suffix::long_literal: return exprs::literal(val, ast::long_type);
		case lexical_token_suffix::unsigned_literal: return exprs::literal(val, ast::uint_type);
		// case lexical_token_suffix::unsigned_long_literal: return exprs::literal(tok.integer(), ast::ulong_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_floating_expr() {
		assert(peek_skip_indent().is_floating_literal() && "expected token to be a floating literal.");
		auto tok = next_skip_indent();
		auto val = tok.floating_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return exprs::literal(val, ast::double_type);
		case lexical_token_suffix::float_literal: return exprs::literal(val, ast::float_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_identifier_expr() {
		assert(peek_skip_indent().is_identifier() && "expected token to be an identifier.");
		auto tok = next_skip_indent();
		auto ident = tok.text();
		return exprs::identifier(_scope, ident);
	}

	std::unique_ptr<ast::binary_expression> parser::parse_binary_expr(std::unique_ptr<ast::expression> lhs) {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto tok = peek_skip_indent();
		std::unique_ptr<ast::binary_expression> expr;

		switch (tok.symbol()) {
		default: return error("unexpected symbol '{}'", tok);
		case symbols::plus:
			if (auto rhs = parse_binary_expr_rhs())
				expr = exprs::addition(std::move(lhs), std::move(rhs));
			else return error("expected right-hand expression of '+' before '{}'", peek_skip_indent());
			break;
		case symbols::minus:
			if (auto rhs = parse_binary_expr_rhs())
				expr = exprs::subtraction(std::move(lhs), std::move(rhs));
			else return error("expected right-hand expression of '-' before '{}'", peek_skip_indent());
			break;
		case symbols::asterisk:
			if (auto rhs = parse_binary_expr_rhs())
				expr = exprs::multiplication(std::move(lhs), std::move(rhs));
			else return error("expected right-hand expression of '*' before '{}'", peek_skip_indent());
			break;
		case symbols::forward_slash:
			if (auto rhs = parse_binary_expr_rhs())
				expr = exprs::division(std::move(lhs), std::move(rhs));
			else return error("expected right-hand expression of '/' before '{}'", peek_skip_indent());
			break;
		}

		return is_binary_op(peek_skip_indent())
			? parse_binary_expr(std::move(expr))
			: std::move(expr);
	}

	std::unique_ptr<ast::expression> parser::parse_binary_expr_rhs() {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto prev = next_skip_indent(); // consume binary operator symbol.
		auto rhs = parse_primary_expr();
		auto curr = peek_skip_indent(); // look-ahead for possible binary operator.

		if (compare_binary_op_precedence(curr, prev) < 0)
			rhs = parse_binary_expr(std::move(rhs));

		return std::move(rhs);
	}
}