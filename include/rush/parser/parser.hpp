#pragma once

#ifndef RUSH_PARSER_HPP
#define RUSH_PARSER_HPP

#include "fmt/format.h"

#include "rush/extra/iterator.hpp"
#include "rush/extra/function_ref.hpp"

#include "rush/ast/types.hpp"
#include "rush/ast/module.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/context.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"

#include "rush/parser/result.hpp"
#include "rush/parser/options.hpp"
#include "rush/parser/symbol.hpp"
#include "rush/parser/scope.hpp"

#include "rush/diag/syntax_error.hpp"

#include <vector>

namespace rush {
	class parser {
		using lxa_iterator = lexical_analysis::const_iterator;
		using lxa_iterator_difference_type = std::iterator_traits<lxa_iterator>::difference_type;

	public:
		explicit parser(parser_options const& opts)
			: _opts { opts }
         , _eof { tokens::eof() }
         , _indent_stack { } {}

		rush::syntax_analysis parse(lexical_analysis const& lxa) {
         initialize(lxa);
         auto result = parse_module();
         return rush::syntax_analysis {
            std::move(_context),
            std::move(result)
         };
		}

	private:
      rush::scope_chain _scope;
		rush::lexical_token _eof;
		rush::parser_options _opts;

      // tracks the current level of indentation along with whether not it was skipped.
      // false = unskipped indentation.
      // true =  skipped indentation.
      std::vector<bool> _indent_stack;

      std::unique_ptr<ast::module> _module;
      std::unique_ptr<ast::context> _context;
		std::pair<lxa_iterator, lxa_iterator> _range;


		void initialize(lexical_analysis const& lxa) {
         if (!lxa.empty()) {
            auto loc = lxa.back().location();
            _eof = tokens::eof(loc.next_column(lxa.back().size()), lxa.source());
            _range = { lxa.begin(), lxa.end() };
         }

         _context = std::make_unique<ast::context>();
         _module = std::make_unique<ast::module>(std::string { lxa.id() });
		}

      bool is_indent_skipped(std::size_t offset = 0) {
         return !_indent_stack.empty()
             && _indent_stack[_indent_stack.size() - offset - 1];
      }

      lexical_token const& peek_with_indent(lxa_iterator_difference_type offset = 0) {
         auto temp = _range.first;
         auto& last = _range.second;
         std::size_t indent_offset = 0;
         for (; temp != last &&
            ((temp->is(symbols::dedent) && is_indent_skipped(indent_offset++)) ||
            offset-- > 0); ++temp) ;
         return temp != last ? *temp : _eof;
		}

		lexical_token const& peek_skip_indent(lxa_iterator_difference_type offset = 0) {
         auto temp = _range.first;
         auto& last = _range.second;
         std::size_t indent_offset = 0;
         for (; temp != last &&
            (temp->is(symbols::indent) ||
            (temp->is(symbols::dedent) && is_indent_skipped(indent_offset++))
            || offset-- > 0); ++temp) ;
         return temp != last ? *temp : _eof;
      }

      lexical_token const& next_with_indent() {
         auto temp = _range.first;
         auto& first = _range.first;
         auto& last = _range.second;

         if (first != last && first->is(symbols::indent)) {
            _indent_stack.push_back(false);
         }

         if (first != last && first->is(symbols::dedent)) {
            if (is_indent_skipped()) { temp = ++first; }
            _indent_stack.pop_back();
         }

         if (first != last) ++first;
         return temp != last ? *temp : _eof;
		}

      bool is_lambda_expr_ahead() {
         auto temp = _range.first;
         auto& last = _range.second;

         ++temp; // skip opening '('
         auto parens = 1;

         // skip all tokens until last closing parenthesis.
         // (it may be less error-prone and more efficient to
         // try and parse the tokens than simply skipping
         // all tokens until the last closing parens is found).
         while (temp != last && parens >= 1) {
            if (temp->is(symbols::left_parenthesis)) { ++parens; }
            else if (temp->is(symbols::right_parenthesis)) { --parens; }
            ++temp;
         }

         // we have a lambda expression if the last closing
         // parenthesis is immediatley followed by a thin or thick arrow.
         return temp->is(symbols::thick_arrow)
             || temp->is(symbols::thin_arrow);
      }

		lexical_token const& next_skip_indent() {
         auto temp = _range.first;
         auto& first = _range.first;
         auto& last = _range.second;

         while (first != last && first->is(symbols::indent)) {
            _indent_stack.push_back(true);
            temp = ++first;
         }

         while (first != last && first->is(symbols::dedent)) {
            if (!is_indent_skipped()) break;
            _indent_stack.pop_back();
            temp = ++first;
         }

         if (first != last) ++first;
         return temp != last ? *temp : _eof;
		}

      bool consume_skip_indent(symbol_token_t sym) {
         if (peek_skip_indent().is(sym)) {
            next_skip_indent();
            return true;
         }
         return false;
      }

      bool consume_skip_indent(keyword_token_t kw) {
         if (peek_skip_indent().is(kw)) {
            next_skip_indent();
            return true;
         }
         return false;
      }

      // modules.
      rush::parse_result<ast::module> parse_module();
      rush::parse_result<ast::import_declaration> parse_import_decl();

      // types.
      rush::parse_type_result parse_type();
      rush::parse_type_result parse_tuple_type();
      rush::parse_type_result parse_simple_type();
		rush::parse_type_result parse_type_annotation();

      rush::parse_type_result parse_array_type(ast::type_ref);
      rush::parse_type_result parse_function_type(ast::type_ref);

		// declarations.
      rush::parse_result<ast::declaration> parse_toplevel_decl();

		rush::parse_result<ast::declaration> _parse_storage_decl(std::string,
			rush::function_ref<std::unique_ptr<ast::declaration>(std::string, ast::type_ref, std::unique_ptr<ast::expression>)>);

		rush::parse_result<ast::declaration> parse_constant_decl();
		rush::parse_result<ast::declaration> parse_variable_decl();
		rush::parse_result<ast::declaration> parse_function_decl();

      rush::parse_result<ast::statement> parse_function_body();
      rush::parse_result<ast::statement> parse_function_expr_body();
      rush::parse_result<ast::statement> parse_function_stmt_body();

      rush::parse_result<ast::parameter_list> parse_parameter_list();

		// statements.
      rush::parse_result<ast::statement> parse_stmt();

      rush::parse_result<ast::statement> parse_pass_stmt();
      rush::parse_result<ast::statement> parse_throw_stmt();
      rush::parse_result<ast::statement> parse_break_stmt();
      rush::parse_result<ast::statement> parse_yield_stmt();
		rush::parse_result<ast::statement> parse_return_stmt();
      rush::parse_result<ast::statement> parse_continue_stmt();

		rush::parse_result<ast::statement> parse_if_stmt();
		rush::parse_result<ast::statement> parse_for_stmt();
      rush::parse_result<ast::statement> parse_else_stmt();
		rush::parse_result<ast::statement> parse_while_stmt();
      rush::parse_result<ast::statement> parse_switch_stmt();
      rush::parse_result<ast::statement> parse_try_stmt();
      rush::parse_result<ast::statement> parse_with_stmt();

		rush::parse_result<ast::statement> parse_block_stmt();
      rush::parse_result<ast::statement> parse_inline_stmt();

      std::pair<rush::parse_result<ast::statement>, bool> parse_simple_stmt(keyword_token_t kw);
      std::pair<rush::parse_result<ast::statement>, bool> parse_compound_stmt(keyword_token_t kw);

		// expressions.
		rush::parse_result<ast::expression> parse_expr();
		rush::parse_result<ast::expression> parse_paren_expr();
		rush::parse_result<ast::expression> parse_primary_expr();
      rush::parse_result<ast::expression> parse_simple_paren_expr();
      rush::parse_result<ast::expression> parse_complex_paren_expr();
      rush::parse_result<ast::expression> parse_array_literal_expr();
      rush::parse_result<ast::expression> parse_tuple_literal_expr();
      rush::parse_result<ast::expression> parse_tuple_literal_expr(rush::parse_result<ast::argument>);
      rush::parse_result<ast::expression> parse_lambda_expr();

		rush::parse_result<ast::expression> parse_string_expr();
		rush::parse_result<ast::expression> parse_integer_expr();
		rush::parse_result<ast::expression> parse_floating_expr();
		rush::parse_result<ast::expression> parse_identifier_expr();
      rush::parse_result<ast::expression> parse_member_access_expr(rush::parse_result<ast::expression>);

		rush::parse_result<ast::expression> parse_unary_expr();
		rush::parse_result<ast::expression> parse_unary_postfix_expr(rush::parse_result<ast::expression>);
		rush::parse_result<ast::expression> parse_binary_expr(rush::parse_result<ast::expression>);
		rush::parse_result<ast::expression> parse_binary_expr_rhs();

      rush::parse_result<ast::expression> parse_ternary_expr(rush::parse_result<ast::expression>);
      rush::parse_result<ast::expression> parse_invoke_expr(rush::parse_result<ast::expression>);

      rush::parse_result<ast::argument> parse_argument();
   // rush::parse_result<ast::binding_list> parse_binding_list();
      rush::parse_result<ast::element_list> parse_element_list();
      rush::parse_result<ast::argument_list> parse_argument_list();
      rush::parse_result<ast::argument_list> parse_argument_list(rush::parse_result<ast::argument>);


      rush::parse_result<ast::declaration> scope_insert(std::unique_ptr<ast::declaration> decl, rush::lexical_token const& ident);


      std::unique_ptr<diag::syntax_error> parse_terminator() {
         auto tok = peek_with_indent();
         if (tok.is(symbols::semi_colon)) {
         while (tok.is(symbols::semi_colon)) {
            next_with_indent();
            tok = peek_with_indent();
         }
         }

         return nullptr;
      }

      template <typename NodeT>
      rush::parse_result<NodeT> terminated(rush::parse_result<NodeT>(parser::*parse_fn)()) {
         auto parse_result = (this->*parse_fn)();
         if (parse_result.failed())
            return std::move(parse_result);

         auto term_result = parse_terminator();
         if (term_result != nullptr)
            return std::move(term_result);

         return std::move(parse_result);
      }
	};
}

#endif // RUSH_PARSER_HPP
