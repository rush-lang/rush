#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/exprs/binary.hpp"
#include "rush/ast/exprs/unary.hpp"

#include "rush/parser/parser.hpp"

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

   std::unique_ptr<ast::expression> parser::parse_initializer() {
      assert(peek_skip_indent().is(symbols::equals) && "expected token to be '='");
      next_skip_indent(); // consume '=' token.
		return parse_expr();
	}

	std::unique_ptr<ast::expression> parser::parse_expr() {
		auto expr = parse_primary_expr();

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
			switch (tok.keyword()) {
			default: return error("unexpected keyword '{}' parsing primary expression", tok);
			case keywords::true_: next_skip_indent(); return exprs::literal(true);
			case keywords::false_: next_skip_indent(); return exprs::literal(false);
			}
		case lexical_token_type::symbol:
			switch (tok.symbol()) {
			default: return error("unexpected symbol '{}' parsing primary expression", tok);
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
		case lexical_token_suffix::none: return exprs::literal(val, ast::types::int_type);
		case lexical_token_suffix::long_literal: return exprs::literal(val, ast::types::long_type);
		case lexical_token_suffix::unsigned_literal: return exprs::literal(val, ast::types::uint_type);
		// case lexical_token_suffix::unsigned_long_literal: return exprs::literal(tok.integer(), ast::ulong_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_floating_expr() {
		assert(peek_skip_indent().is_floating_literal() && "expected token to be a floating literal.");
		auto tok = next_skip_indent();
		auto val = tok.floating_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return exprs::literal(val, ast::types::double_type);
		case lexical_token_suffix::float_literal: return exprs::literal(val, ast::types::float_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_identifier_expr() {
		assert(peek_skip_indent().is_identifier() && "expected token to be an identifier.");

		auto tok = next_skip_indent();
      auto sym = _scope.current().lookup(tok.text());
      if (sym.is_undefined())
         return error("the name '{}' does not exist in the current context.", tok);

      return exprs::identifier(sym.declaration()->identifier());
	}

	std::unique_ptr<ast::binary_expression> parser::parse_binary_expr(std::unique_ptr<ast::expression> lhs) {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto tok = peek_skip_indent();
		auto rhs = parse_binary_expr_rhs();
      if (!lhs || !rhs) return nullptr;

		std::unique_ptr<ast::binary_expression> expr;

		switch (tok.symbol()) {
		default: return error("binary operator '{}' not yet supported", tok);
		case symbols::plus: expr = exprs::addition(std::move(lhs), std::move(rhs)); break;
		case symbols::minus: expr = exprs::subtraction(std::move(lhs), std::move(rhs)); break;
		case symbols::percent: expr = exprs::modulo(std::move(lhs), std::move(rhs)); break;
		case symbols::asterisk: expr = exprs::multiplication(std::move(lhs), std::move(rhs)); break;
		case symbols::forward_slash: expr = exprs::division(std::move(lhs), std::move(rhs)); break;
		case symbols::double_pipe: expr = exprs::logical_or(std::move(lhs), std::move(rhs)); break;
		case symbols::double_ampersand: expr = exprs::logical_and(std::move(lhs), std::move(rhs)); break;
		case symbols::left_chevron: expr = exprs::less_than(std::move(lhs), std::move(rhs)); break;
		case symbols::right_chevron: expr = exprs::greater_than(std::move(lhs), std::move(rhs)); break;
		case symbols::left_chevron_equals: expr = exprs::less_equals(std::move(lhs), std::move(rhs)); break;
		case symbols::right_chevron_equals: expr = exprs::greater_equals(std::move(lhs), std::move(rhs)); break;
		}

		return is_binary_op(peek_skip_indent())
			? parse_binary_expr(std::move(expr))
			: std::move(expr);
	}

	std::unique_ptr<ast::expression> parser::parse_binary_expr_rhs() {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto prev = next_skip_indent(); // consume binary operator symbol.
      auto next = peek_skip_indent(); // store the next token.

		if (auto rhs = parse_primary_expr()) {
         next = peek_skip_indent(); // look-ahead for possible binary operator.

         if (compare_binary_op_precedence(next, prev) < 0)
            rhs = parse_binary_expr(std::move(rhs));

         return std::move(rhs);
      }

      return error("expected right-hand expression of '{1}' before '{0}'", next, prev);
	}
}
