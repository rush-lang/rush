#include "rush/ast/expr/identifier.hpp"
#include "rush/ast/expr/literal.hpp"
#include "rush/ast/expr/binary.hpp"
#include "rush/ast/expr/unary.hpp"

#include "parser.hpp"

namespace rush {

	std::unique_ptr<ast::expression> parser::parse_expression() {
		return this->parse_primary_expression();
	}

	std::unique_ptr<ast::expression> parser::parse_parenthesised_expression() {
		assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected token to be \'(\'");
		next_skip_indent(); // consume '('
		auto expr = this->parse_expression();
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
		case lexical_token_type::identifier: return this->parse_identifier_expression();
		case lexical_token_type::string_literal: return this->parse_string_expression();
		case lexical_token_type::integer_literal: return this->parse_integer_expression();
		case lexical_token_type::floating_literal: return this->parse_floating_expression();
		case lexical_token_type::keyword:
			if (tok.is(keywords::true_)) return ast::literal_expr(true);
			if (tok.is(keywords::false_)) return ast::literal_expr(false);
			return error("unexpected keyword '{}' when parsing primary expression", tok);
		case lexical_token_type::symbol:
			switch (tok.symbol()) {
			default: return error("unexpected symbol '{}' in expression", tok);
			case symbols::left_parenthesis: return this->parse_parenthesised_expression();
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
		return ast::identifier_expr(ident);
	}
}