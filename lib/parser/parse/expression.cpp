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

	rush::parse_result<ast::expression> parser::parse_expr() {
		auto result = parse_primary_expr();
      if (result.failed()) return std::move(result);

		if (is_binary_op(peek_skip_indent()))
			result = parse_binary_expr(std::move(result));

      if (peek_skip_indent().is(symbols::question_mark))
         result = parse_ternary_expr(std::move(result));

		return std::move(result);
	}

	rush::parse_result<ast::expression> parser::parse_paren_expr() {
		assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected token to be \'(\'");
		next_skip_indent(); // consume '('
		auto result = parse_expr();
		if (result.failed()) return std::move(result);

		auto tok = peek_skip_indent();
		if (tok.is(symbols::right_parenthesis)) {
			next_skip_indent(); // consume ')'
			return std::move(result);
		}

		return error("expected a closing \')\' before \'{}\'", tok);
	}

	rush::parse_result<ast::expression> parser::parse_primary_expr() {
		auto tok = peek_skip_indent();

      rush::parse_result<ast::expression> result;
		switch (tok.type()) {
		default: return error("expected primary expression, but found '{}'", tok);
		case lexical_token_type::identifier: result = parse_identifier_expr(); break;
		case lexical_token_type::string_literal: result = parse_string_expr(); break;
		case lexical_token_type::integer_literal: result = parse_integer_expr(); break;
		case lexical_token_type::floating_literal: result = parse_floating_expr(); break;
		case lexical_token_type::keyword:
			switch (tok.keyword()) {
			default: return error("unexpected keyword '{}' parsing primary expression", tok);
			case keywords::true_: next_skip_indent(); result = exprs::literal(true, _context->bool_type()); break;
			case keywords::false_: next_skip_indent(); result = exprs::literal(false, _context->bool_type()); break;
			} break;
		case lexical_token_type::symbol:
         if (is_unary_prefix_op(tok)) result = parse_unary_expr();
         else switch (tok.symbol()) {
         case symbols::left_parenthesis: result = parse_paren_expr(); break;
         default: return error("unexpected symbol '{}' parsing primary expression", tok);
         } break;
		}

      return is_unary_postfix_op(peek_skip_indent())
         ? parse_unary_postfix_expr(std::move(result))
         : std::move(result);
	}

	rush::parse_result<ast::expression> parser::parse_string_expr() {
		assert(peek_skip_indent().is_string_literal() && "expected token to be a string literal.");
		auto tok = next_skip_indent();
		auto str = tok.text();
		return exprs::literal(str, _context->string_type());
	}

	rush::parse_result<ast::expression> parser::parse_integer_expr() {
		assert(peek_skip_indent().is_integer_literal() && "expected token to be an integer literal.");
		auto tok = next_skip_indent();
		auto val = tok.integer_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return exprs::literal(val, _context->int32_type());
		case lexical_token_suffix::long_literal: return exprs::literal(val, _context->int64_type());
		case lexical_token_suffix::unsigned_literal: return exprs::literal(val, _context->uint32_type());
		case lexical_token_suffix::unsigned_long_literal: return exprs::literal(val, _context->uint64_type());
		default: throw;
		}
	}

	rush::parse_result<ast::expression> parser::parse_floating_expr() {
		assert(peek_skip_indent().is_floating_literal() && "expected token to be a floating literal.");
		auto tok = next_skip_indent();
		auto val = tok.floating_value();
		switch (tok.suffix()) {
		case lexical_token_suffix::none: return exprs::literal(val, _context->ieee64_type());
		case lexical_token_suffix::float_literal: return exprs::literal(val, _context->ieee32_type());
		default: throw;
		}
	}

	rush::parse_result<ast::expression> parser::parse_identifier_expr() {
		assert(peek_skip_indent().is_identifier() && "expected token to be an identifier.");

		auto tok = next_skip_indent();
      auto sym = _scope.current().lookup(tok.text());

      return !sym.is_undefined()
         ? exprs::identifier(sym.declaration()->identifier())
         : exprs::identifier(_scope.resolver(tok.text()));
	}


   rush::parse_result<ast::expression> parser::parse_unary_expr() {
      assert(is_unary_prefix_op(peek_skip_indent()) && "expected unary operator.");

      auto tok = next_skip_indent(); // consume unary operator token.
      auto operand_result = parse_primary_expr();
      if (operand_result.failed())
         return std::move(operand_result);

      if (is_unary_postfix_op(peek_skip_indent())) {
         operand_result = parse_unary_postfix_expr(std::move(operand_result));
         if (operand_result.failed()) return std::move(operand_result);
      }

      switch (tok.symbol()) {
      default: return error("unary operator '{}' not yet supported", tok);
      case symbols::plus: return exprs::positive(std::move(operand_result));
      case symbols::minus: return exprs::negative(std::move(operand_result));
      case symbols::plus_plus: return exprs::pre_increment(std::move(operand_result));
      case symbols::minus_minus: return exprs::pre_decrement(std::move(operand_result));
      case symbols::exclamation_mark: return exprs::logical_negation(std::move(operand_result));
      case symbols::tilde: return exprs::bitwise_negation(std::move(operand_result));
      }
   }

   rush::parse_result<ast::expression> parser::parse_unary_postfix_expr(rush::parse_result<ast::expression> operand_result) {
      assert(is_unary_postfix_op(peek_skip_indent()) && "expected unary postfix operator.");
      if (operand_result.failed()) return nullptr;

      auto tok = peek_skip_indent();
      switch (tok.symbol()) {
      default: return error("unary postfix operator not yet supported.", tok);

      case symbols::plus_plus:
         operand_result = exprs::post_increment(std::move(operand_result));
         next_skip_indent();
         break;

      case symbols::minus_minus:
         operand_result = exprs::post_decrement(std::move(operand_result));
         next_skip_indent();
         break;

      case symbols::left_parenthesis:
         operand_result = parse_invocation_expr(std::move(operand_result));
         if (operand_result.failed()) return std::move(operand_result);
         break;
      }

      return is_unary_postfix_op(peek_skip_indent())
         ? parse_unary_postfix_expr(std::move(operand_result))
         : std::move(operand_result);
   }

	rush::parse_result<ast::expression> parser::parse_binary_expr(rush::parse_result<ast::expression> lhs) {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto tok = peek_skip_indent();
		auto rhs = parse_binary_expr_rhs();

      if (lhs.failed() || rhs.failed()) {
         lhs = std::move(rhs);
         return std::move(lhs);
      }

		rush::parse_result<ast::binary_expression> result;

      if (tok.is_symbol()) {
		switch (tok.symbol()) {
		default: return error("binary operator '{}' not yet supported", tok);
      case symbols::equals: result = exprs::assignment(std::move(lhs), std::move(rhs)); break;

      // arithmetic binary operators
		case symbols::plus: result = exprs::addition(std::move(lhs), std::move(rhs)); break;
		case symbols::minus: result = exprs::subtraction(std::move(lhs), std::move(rhs)); break;
		case symbols::percent: result = exprs::modulo(std::move(lhs), std::move(rhs)); break;
		case symbols::asterisk: result = exprs::multiplication(std::move(lhs), std::move(rhs)); break;
		case symbols::forward_slash: result = exprs::division(std::move(lhs), std::move(rhs)); break;

      // compound arithmetic binary operators
      case symbols::plus_equals: result = exprs::compound_addition(std::move(lhs), std::move(rhs)); break;
      case symbols::minus_equals: result = exprs::compound_subtraction(std::move(lhs), std::move(rhs)); break;
      case symbols::percent_equals: result = exprs::compound_modulo(std::move(lhs), std::move(rhs)); break;
      case symbols::asterisk_equals: result = exprs::compound_multiplication(std::move(lhs), std::move(rhs)); break;
      case symbols::forward_slash_equals: result = exprs::compound_division(std::move(lhs), std::move(rhs)); break;

      // bit-wise binary operators
      case symbols::pipe: result = exprs::bitwise_or(std::move(lhs), std::move(rhs)); break;
      case symbols::caret: result = exprs::bitwise_xor(std::move(lhs), std::move(rhs)); break;
      case symbols::ampersand: result = exprs::bitwise_and(std::move(lhs), std::move(rhs)); break;
      case symbols::double_left_chevron: result = exprs::left_shift(std::move(lhs), std::move(rhs)); break;
      case symbols::double_right_chevron: result = exprs::right_shift(std::move(lhs), std::move(rhs)); break;

      // compound bit-wise binary operators
      case symbols::pipe_equals: result = exprs::compound_bitwise_or(std::move(lhs), std::move(rhs)); break;
      case symbols::caret_equals: result = exprs::compound_bitwise_xor(std::move(lhs), std::move(rhs)); break;
      case symbols::ampersand_equals: result = exprs::compound_bitwise_and(std::move(lhs), std::move(rhs)); break;
      case symbols::double_left_chevron_equals: result = exprs::compound_left_shift(std::move(lhs), std::move(rhs)); break;
      case symbols::double_right_chevron_equals: result = exprs::compound_right_shift(std::move(lhs), std::move(rhs)); break;

      // special operators
      case symbols::ellipses: result = exprs::range(std::move(lhs), std::move(rhs)); break;

      // logical binary operators
		case symbols::double_pipe: result = exprs::logical_or(std::move(lhs), std::move(rhs)); break;
		case symbols::double_ampersand: result = exprs::logical_and(std::move(lhs), std::move(rhs)); break;
      case symbols::equals_equals: result = exprs::equal(std::move(lhs), std::move(rhs)); break;
      case symbols::exclamation_mark_equals: result = exprs::not_equal(std::move(lhs), std::move(rhs)); break;
		case symbols::left_chevron: result = exprs::less_than(std::move(lhs), std::move(rhs)); break;
		case symbols::right_chevron: result = exprs::greater_than(std::move(lhs), std::move(rhs)); break;
		case symbols::left_chevron_equals: result = exprs::less_equals(std::move(lhs), std::move(rhs)); break;
		case symbols::right_chevron_equals: result = exprs::greater_equals(std::move(lhs), std::move(rhs)); break;
		}
      } else {
         switch (tok.keyword()) {
         default: return error("binary operator '{}' not yet supported", tok);
         // case keywords::is_: result = exprs::is(std::move(lhs), std::move(rhs)); break;
         // case keywords::as_: result = exprs::as(std::move(lhs), std::move(rhs)); break;
         }
      }

		return is_binary_op(peek_skip_indent())
			? parse_binary_expr(std::move(result))
			: std::move(result);
	}

	rush::parse_result<ast::expression> parser::parse_binary_expr_rhs() {
		assert(is_binary_op(peek_skip_indent()) && "expected binary operator.");

		auto prev = next_skip_indent(); // consume binary operator symbol.
      auto next = peek_skip_indent(); // store the next token.

      auto rhs_result = parse_primary_expr();
		if (rhs_result.success()) {
         next = peek_skip_indent(); // look-ahead for possible binary operator.

         if (is_binary_op(next)) {
            auto opcmp = compare_binary_op_precedence(next, prev);
            if (opcmp < 0 || (opcmp == 0 && binary_associativity(next) > 0))
               rhs_result = parse_binary_expr(std::move(rhs_result));
         }

         return std::move(rhs_result);
      }

      return std::move(rhs_result);
      // return error("expected right-hand expression of '{1}' before '{0}'", next, prev);
	}

   rush::parse_result<ast::expression> parser::parse_ternary_expr(rush::parse_result<ast::expression> cond_result) {
      assert(peek_skip_indent().is(symbols::question_mark) && "expected ternary expression sequence.");
      next_skip_indent(); // consume '?' symbol.

      auto true_result = parse_expr();
      if (true_result.failed()) return std::move(true_result);

      if (peek_skip_indent().is_not(symbols::colon))
         return error("syntax error, expected ':'", next_skip_indent());
      next_skip_indent(); // consume ':' symbol.

      auto false_result = parse_expr();
      if (false_result.failed()) return std::move(false_result);

      return exprs::ternary(
         std::move(cond_result),
         std::move(true_result),
         std::move(false_result));
   }

   rush::parse_result<ast::argument_list> parser::parse_argument_list() {
      assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected '(' start of argument list.");
		next_skip_indent(); // consume '(' symbol

      if (peek_skip_indent().is(symbols::right_parenthesis)) {
			next_skip_indent(); // consume ')' symbol
         return exprs::arg_list();
		}

      std::vector<rush::parse_result<ast::argument>> results;
      do {
         auto expr_result = parse_expr();
         if (expr_result.failed())
            return std::move(expr_result).as<ast::argument_list>();

         results.push_back(exprs::arg(std::move(expr_result)));
      } while (consume_skip_indent(symbols::comma));

      if (peek_skip_indent().is_not(symbols::right_parenthesis))
         return error("expected ')' before '{}'.", next_skip_indent());
      next_skip_indent();

      std::vector<std::unique_ptr<ast::argument>> args;
      std::move(results.begin(), results.end(), std::back_inserter(args));
      return exprs::arg_list(std::move(args));
   }


   rush::parse_result<ast::expression> parser::parse_invocation_expr(rush::parse_result<ast::expression> expr_result) {
      assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected ternary expression sequence.");

      auto result = parse_argument_list();
      if (result.failed())
         return std::move(result).as<ast::expression>();

		return exprs::call(
         std::move(expr_result),
         std::move(result));
   }
}
