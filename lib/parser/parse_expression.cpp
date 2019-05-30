#include "rush/ast/expressions.hpp"

#include "rush/parser/parser.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;

namespace rush {

#define RUSH_IS_UNARY_PREFIX_OP_FUNC
#define RUSH_IS_UNARY_POSTFIX_OP_FUNC
#define RUSH_IS_BINARY_OP_FUNC
#define RUSH_PRECEDENCE_FUNC
#define RUSH_ASSOCIATIVITY_FUNC
#include "rush/ast/exprs/_operators.hpp"

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
      if (!expr) return nullptr;

      if (is_unary_postfix_op(peek_skip_indent()))
         expr = parse_unary_postfix_expr(std::move(expr));

		if (peek_skip_indent().is(symbols::left_parenthesis))
			expr = parse_invocation_expr(std::move(expr));

		if (is_binary_op(peek_skip_indent()))
			expr = parse_binary_expr(std::move(expr));

      if (peek_skip_indent().is(symbols::question_mark))
         expr = parse_ternary_expr(std::move(expr));

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
         if (is_unary_prefix_op(tok)) return parse_unary_expr();
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
		case lexical_token_suffix::none: return exprs::literal(val, ast::types::int32_type);
		case lexical_token_suffix::long_literal: return exprs::literal(val, ast::types::int64_type);
		case lexical_token_suffix::unsigned_literal: return exprs::literal(val, ast::types::uint32_type);
		// case lexical_token_suffix::unsigned_long_literal: return exprs::literal(tok.integer(), ast::uint64_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_floating_expr() {
		assert(peek_skip_indent().is_floating_literal() && "expected token to be a floating literal.");
		auto tok = next_skip_indent();
		auto val = tok.floating_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return exprs::literal(val, ast::types::ieee64_type);
		case lexical_token_suffix::float_literal: return exprs::literal(val, ast::types::ieee32_type);
		default: throw;
		}
	}

	std::unique_ptr<ast::expression> parser::parse_identifier_expr() {
		assert(peek_skip_indent().is_identifier() && "expected token to be an identifier.");

		auto tok = next_skip_indent();
      auto sym = _scope.current().lookup(tok.text());

      return !sym.is_undefined()
         ? exprs::identifier(sym.declaration()->identifier())
         : exprs::identifier(_scope.resolver(tok.text()));
	}


   std::unique_ptr<ast::expression> parser::parse_unary_expr() {
      assert(is_unary_prefix_op(peek_skip_indent()) && "expected unary operator.");

      auto tok = next_skip_indent(); // consume unary operator token.
      auto operand = parse_primary_expr();
      if (is_unary_postfix_op(peek_skip_indent()))
         operand = parse_unary_postfix_expr(std::move(operand));

      switch (tok.symbol()) {
      default: return error("unary operator '{}' not yet supported", tok);
      case symbols::plus: return exprs::positive(std::move(operand));
      case symbols::minus: return exprs::negative(std::move(operand));
      case symbols::plus_plus: return exprs::pre_increment(std::move(operand));
      case symbols::minus_minus: return exprs::pre_decrement(std::move(operand));
      case symbols::exclamation_mark: return exprs::logical_negation(std::move(operand));
      case symbols::tilde: return exprs::bitwise_negation(std::move(operand));
      }
   }

   std::unique_ptr<ast::expression> parser::parse_unary_postfix_expr(std::unique_ptr<ast::expression> op) {
      assert(is_unary_postfix_op(peek_skip_indent()) && "expected unary postfix operator.");
      if (!op) return nullptr;

      auto tok = next_skip_indent();
      switch (tok.symbol()) {
      default: return error("unary postfix operator not yet supported.", tok);
      case symbols::plus_plus: op = exprs::post_increment(std::move(op)); break;
      case symbols::minus_minus: op = exprs::post_decrement(std::move(op)); break;
      }

      return is_unary_postfix_op(peek_skip_indent())
         ? parse_unary_postfix_expr(std::move(op))
         : std::move(op);
   }

	std::unique_ptr<ast::expression> parser::parse_binary_expr(std::unique_ptr<ast::expression> lhs) {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto tok = peek_skip_indent();
		auto rhs = parse_binary_expr_rhs();
      if (!lhs || !rhs) return nullptr;

		std::unique_ptr<ast::binary_expression> expr;

      if (tok.is_symbol()) {
		switch (tok.symbol()) {
		default: return error("binary operator '{}' not yet supported", tok);
      case symbols::equals: expr = exprs::assignment(std::move(lhs), std::move(rhs)); break;

      // arithmetic binary operators
		case symbols::plus: expr = exprs::addition(std::move(lhs), std::move(rhs)); break;
		case symbols::minus: expr = exprs::subtraction(std::move(lhs), std::move(rhs)); break;
		case symbols::percent: expr = exprs::modulo(std::move(lhs), std::move(rhs)); break;
		case symbols::asterisk: expr = exprs::multiplication(std::move(lhs), std::move(rhs)); break;
		case symbols::forward_slash: expr = exprs::division(std::move(lhs), std::move(rhs)); break;

      // compound arithmetic binary operators
      case symbols::plus_equals: expr = exprs::compound_addition(std::move(lhs), std::move(rhs)); break;
      case symbols::minus_equals: expr = exprs::compound_subtraction(std::move(lhs), std::move(rhs)); break;
      case symbols::percent_equals: expr = exprs::compound_modulo(std::move(lhs), std::move(rhs)); break;
      case symbols::asterisk_equals: expr = exprs::compound_multiplication(std::move(lhs), std::move(rhs)); break;
      case symbols::forward_slash_equals: expr = exprs::compound_division(std::move(lhs), std::move(rhs)); break;

      // bit-wise binary operators
      case symbols::pipe: expr = exprs::bitwise_or(std::move(lhs), std::move(rhs)); break;
      case symbols::caret: expr = exprs::bitwise_xor(std::move(lhs), std::move(rhs)); break;
      case symbols::ampersand: expr = exprs::bitwise_and(std::move(lhs), std::move(rhs)); break;
      case symbols::double_left_chevron: expr = exprs::left_shift(std::move(lhs), std::move(rhs)); break;
      case symbols::double_right_chevron: expr = exprs::right_shift(std::move(lhs), std::move(rhs)); break;

      // compound bit-wise binary operators
      case symbols::pipe_equals: expr = exprs::compound_bitwise_or(std::move(lhs), std::move(rhs)); break;
      case symbols::caret_equals: expr = exprs::compound_bitwise_xor(std::move(lhs), std::move(rhs)); break;
      case symbols::ampersand_equals: expr = exprs::compound_bitwise_and(std::move(lhs), std::move(rhs)); break;
      case symbols::double_left_chevron_equals: expr = exprs::compound_left_shift(std::move(lhs), std::move(rhs)); break;
      case symbols::double_right_chevron_equals: expr = exprs::compound_right_shift(std::move(lhs), std::move(rhs)); break;

      // logical binary operators
		case symbols::double_pipe: expr = exprs::logical_or(std::move(lhs), std::move(rhs)); break;
		case symbols::double_ampersand: expr = exprs::logical_and(std::move(lhs), std::move(rhs)); break;
      case symbols::equals_equals: expr = exprs::equal(std::move(lhs), std::move(rhs)); break;
      case symbols::exclamation_mark_equals: expr = exprs::not_equal(std::move(lhs), std::move(rhs)); break;
		case symbols::left_chevron: expr = exprs::less_than(std::move(lhs), std::move(rhs)); break;
		case symbols::right_chevron: expr = exprs::greater_than(std::move(lhs), std::move(rhs)); break;
		case symbols::left_chevron_equals: expr = exprs::less_equals(std::move(lhs), std::move(rhs)); break;
		case symbols::right_chevron_equals: expr = exprs::greater_equals(std::move(lhs), std::move(rhs)); break;
		}
      } else {
         switch (tok.keyword()) {
         default: return error("binary operator '{}' not yet supported", tok);
         // case keywords::is_: expr = exprs::is(std::move(lhs), std::move(rhs)); break;
         // case keywords::as_: expr = exprs::as(std::move(lhs), std::move(rhs)); break;
         }
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

         if (is_binary_op(next)) {
            auto opcmp = compare_binary_op_precedence(next, prev);
            if (opcmp < 0 || (opcmp == 0 && binary_associativity(next) > 0))
               rhs = parse_binary_expr(std::move(rhs));
         }

         return std::move(rhs);
      }

      return error("expected right-hand expression of '{1}' before '{0}'", next, prev);
	}

   std::unique_ptr<ast::expression> parser::parse_ternary_expr(std::unique_ptr<ast::expression> cond) {
      assert(peek_skip_indent().is(symbols::question_mark) && "expected ternary expression sequence.");
      next_skip_indent(); // consume '?' symbol.

      auto true_ = parse_expr();
      if (!true_) return nullptr;

      if (peek_skip_indent().is_not(symbols::colon))
         return error("syntax error, expected ':'", next_skip_indent());
      next_skip_indent(); // consume ':' symbol.

      auto false_ = parse_expr();
      if (!false_) return nullptr;

      return exprs::ternary(
         std::move(cond),
         std::move(true_),
         std::move(false_));
   }

   std::unique_ptr<ast::argument_list> parser::parse_argument_list() {
      assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected ternary expression sequence.");
		next_skip_indent(); // consume '(' symbol

      if (peek_skip_indent().is(symbols::right_parenthesis)) {
			next_skip_indent(); // consume ')' symbol
         return exprs::arg_list();
		}

      std::vector<std::unique_ptr<ast::argument>> args;
      do {
         auto expr = parse_expr();
         if (!expr) return nullptr;

         args.push_back(exprs::arg(std::move(expr)));
      } while (consume_skip_indent(symbols::comma));

      if (peek_skip_indent().is_not(symbols::right_parenthesis))
         return error("expected ')' before '{}'.", next_skip_indent());
      next_skip_indent();

      return exprs::arg_list(std::move(args));
   }


   std::unique_ptr<ast::expression> parser::parse_invocation_expr(std::unique_ptr<ast::expression> fn) {
      assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected ternary expression sequence.");

      auto args = parse_argument_list();
		return exprs::invocation(
         std::move(fn),
         std::move(args));
   }
}
