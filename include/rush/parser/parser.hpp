/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#pragma once

#ifndef RUSH_PARSER_HPP
#define RUSH_PARSER_HPP

#include "fmt/format.h"

#include "rush/extra/iterator.hpp"
#include "rush/extra/function_ref.hpp"

#include "rush/ast/types.hpp"
#include "rush/ast/module.hpp"
#include "rush/ast/source.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/patterns.hpp"
#include "rush/ast/context.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/result.hpp"

#include "rush/parser/result.hpp"
#include "rush/parser/options.hpp"

#include "rush/diag/syntax_error.hpp"

#include <vector>

namespace rush {
   class string_parser;

	class parser {
      friend class string_parser;
		using lxa_iterator = lexical_analysis::const_iterator;
		using lxa_iterator_difference_type = std::iterator_traits<lxa_iterator>::difference_type;

	public:
		explicit parser(parser_options const& opts)
			: _opts { opts }
         , _eof { tokens::eof() }
         , _indent_stack { } {}

		rush::syntax_analysis parse(lexical_analysis const& lxa, ast::context& ctx) {
         initialize(lxa, ctx);

         parse_result<ast::module_node> result;
         try { result = parse_module(); }
         catch (...) { result = diag::errs::internal_parse_error(_eof); }
         return rush::syntax_analysis { std::move(result) };
		}

	private:
		rush::parser_options _opts;
		rush::lexical_token _eof;

      // tracks the current level of indentation along with whether not it was skipped.
      // false = unskipped indentation.
      // true =  skipped indentation.
      std::vector<bool> _indent_stack;

      ast::context* _context;
		std::pair<lxa_iterator, lxa_iterator> _range;


      std::pair<lxa_iterator, lxa_iterator> inject(lxa_iterator first, lxa_iterator last) {
         auto temp = _range;
         _range.first = first;
         _range.second = last;
         return temp;
      }

		void initialize(lexical_analysis const& lxa, ast::context& ctx) {
         if (!lxa.empty()) {
            auto loc = lxa.back().location();
            _eof = tokens::eof(loc.next_column(lxa.back().size()), lxa.source());
            _range = { lxa.begin(), lxa.end() }; }
         _context = &ctx;
		}

      bool is_indent_skipped(std::size_t offset = 0) {
         return is_indent_skipped(
            _indent_stack,
            offset);
      }

      lexical_token const& peek_skip_indent(lxa_iterator_difference_type offset = 0) {
         return peek_skip_indent(
            _range.first,
            _range.second,
            offset);
      }

      lexical_token const& next_skip_indent() {
         return next_skip_indent(
            _range.first,
            _range.second,
            _indent_stack);
		}

      bool is_indent_skipped(std::vector<bool>& indent_stack, std::size_t offset = 0) {
         return !indent_stack.empty()
             && indent_stack[indent_stack.size() - offset - 1];
      }

      lexical_token const& peek_with_lbreak(lxa_iterator_difference_type offset = 0) {
         return peek_with_lbreak(
            _range.first,
            _range.second,
            offset);
      }

      lexical_token const& peek_with_lbreak(lxa_iterator& first, lxa_iterator const& last, lxa_iterator_difference_type offset = 0) {
         auto temp = first;
         std::size_t indent_offset = 0;
         for (; temp != last &&
            (temp->is_comment()) ||
            (temp->is(symbols::indent)) ||
            ((temp->is(symbols::dedent) && is_indent_skipped(indent_offset++))
            || offset-- > 0); ++temp) ;
         return temp != last ? *temp : _eof;
      }

      lexical_token const& peek_with_indent(lxa_iterator_difference_type offset = 0) {
         auto temp = _range.first;
         auto& last = _range.second;
         std::size_t indent_offset = 0;
         for (; temp != last &&
            (temp->is_comment()) ||
            (temp->is(symbols::lbreak)) ||
            ((temp->is(symbols::dedent) && is_indent_skipped(indent_offset++)) ||
            offset-- > 0); ++temp) ;
         return temp != last ? *temp : _eof;
		}

      lexical_token const& peek_skip_indent(lxa_iterator& first, lxa_iterator const& last, lxa_iterator_difference_type offset = 0) {
         auto temp = first;
         std::size_t indent_offset = 0;
         for (; temp != last &&
            (temp->is_comment()) ||
            (temp->is(symbols::lbreak)) ||
            (temp->is(symbols::indent) ||
            (temp->is(symbols::dedent) && is_indent_skipped(indent_offset++))
            || offset-- > 0); ++temp) ;
         return temp != last ? *temp : _eof;
      }

      lexical_token const& next_with_indent() {
         auto temp = _range.first;
         auto& first = _range.first;
         auto& last = _range.second;

         while (first != last && first->is_comment()) {
            temp = ++first;
         }

         while (first != last && first->is(symbols::lbreak)) {
            temp = ++first;
         }

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

      lexical_token const& next_skip_indent(lxa_iterator& first, lxa_iterator const& last, std::vector<bool>& indent_stack) {
         auto temp = first;

         while (first != last && first->is_comment()) {
            temp = ++first;
         }

         while (first != last && first->is(symbols::lbreak)) {
            temp = ++first;
         }

         while (first != last && first->is(symbols::indent)) {
            indent_stack.push_back(true);
            temp = ++first;
         }

         while (first != last && first->is(symbols::dedent)) {
            if (!is_indent_skipped(indent_stack)) break;
            indent_stack.pop_back();
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

      bool is_lambda_expr_ahead() {
         auto temp = _range.first;
         auto& last = _range.second;
         auto indent_stack = _indent_stack;

         // skip opening '('
         auto opening = next_skip_indent(temp, last, indent_stack);
         auto parens = 1;

         symbols::symbol_token_t closing;
         switch (opening.symbol()) {
         case symbols::left_parenthesis: closing = symbols::right_parenthesis; break;
         case symbols::left_bracket: closing = symbols::right_bracket; break;
         case symbols::left_brace: closing = symbols::right_brace; break;
         default: assert("unreachable!");
         }

         // skip all tokens until last closing parenthesis.
         // (it may be less error-prone and more efficient to
         // try and parse the tokens than simply skipping
         // all tokens until the last closing parens is found).
         while (temp != last && parens >= 1) {
            if (temp->is(opening.symbol())) { ++parens; }
            else if (temp->is(closing)) { --parens; }
            next_skip_indent(temp, last, indent_stack);
         }

         // we have a lambda expression if the last closing
         // parenthesis is immediatley followed by a thin or thick arrow.
         return peek_skip_indent(temp, last).is(symbols::thick_arrow)
             || peek_skip_indent(temp, last).is(symbols::colon_equals)
             || peek_skip_indent(temp, last).is(symbols::thin_arrow);
      }

      // modules.
      rush::parse_result<ast::module_node> parse_module();
      rush::parse_result<ast::source_node> parse_source();
      rush::parse_result<ast::import_declaration> parse_import_decl();

      // types.
      rush::parse_type_result parse_type();
      rush::parse_type_result parse_array_type();
      rush::parse_type_result parse_tuple_type();
      rush::parse_type_result parse_simple_type();
		rush::parse_type_result parse_type_annotation();

      rush::parse_type_result parse_function_type(ast::type_ref);

		// declarations.
      rush::parse_result<ast::declaration> parse_toplevel_decl();

		rush::parse_result<ast::declaration> _parse_storage_decl(
         rush::function_ref<std::unique_ptr<ast::declaration>(std::unique_ptr<ast::pattern>)>);

		rush::parse_result<ast::declaration> parse_constant_decl();
		rush::parse_result<ast::declaration> parse_variable_decl();
      rush::parse_result<ast::declaration> parse_iteration_decl();
		rush::parse_result<ast::declaration> parse_function_decl();

      rush::parse_result<ast::statement> parse_function_body();
      rush::parse_result<ast::statement> parse_function_expr_body();
      rush::parse_result<ast::statement> parse_function_stmt_body();
      rush::parse_result<ast::pattern> parse_parameter_list();

      rush::parse_result<ast::declaration> parse_class_decl();
      rush::parse_result<ast::declaration> parse_struct_decl();
      rush::parse_result<ast::member_declaration> parse_member_decl();
      rush::parse_result<ast::member_section_declaration> parse_member_section(ast::member_access);
      rush::parse_result<ast::member_declaration> parse_property_getter();
      rush::parse_result<ast::member_declaration> parse_property_setter();


      rush::parse_result<ast::pattern> parse_pattern();
      rush::parse_result<ast::pattern> parse_storage_pattern();
      rush::parse_result<ast::pattern> parse_iteration_pattern();
      rush::parse_result<ast::pattern> parse_parameter_pattern();

      rush::parse_result<ast::pattern> parse_named_pattern();
      rush::parse_result<ast::pattern> parse_discard_pattern();
      rush::parse_result<ast::pattern> parse_destructure_pattern();
      rush::parse_result<ast::pattern> parse_array_destructure_pattern();
      rush::parse_result<ast::pattern> parse_object_destructure_pattern();
      rush::parse_result<ast::pattern> parse_type_annotation_pattern(rush::parse_result<ast::pattern>);
      rush::parse_result<ast::pattern> parse_binding_pattern(rush::parse_result<ast::pattern>);
      rush::parse_result<ast::pattern> parse_rest_pattern();

      rush::parse_result<ast::pattern> parse_pattern_list(
         rush::function_ref<rush::parse_result<ast::pattern>()> parseFn);

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
      rush::parse_result<ast::expression_list> parse_expr_list();

		rush::parse_result<ast::expression> parse_paren_expr();
		rush::parse_result<ast::expression> parse_primary_expr();
      rush::parse_result<ast::expression> parse_simple_paren_expr();
      rush::parse_result<ast::expression> parse_complex_paren_expr();
      rush::parse_result<ast::expression> parse_array_literal_expr();
      rush::parse_result<ast::expression> parse_tuple_literal_expr();
      rush::parse_result<ast::expression> parse_tuple_literal_expr(rush::parse_result<ast::expression>);
      rush::parse_result<ast::expression> parse_tuple_element_expr();
      rush::parse_result<ast::expression_list> parse_tuple_element_list(rush::parse_result<ast::expression>);
      rush::parse_result<ast::expression> parse_lambda_expr();

		rush::parse_result<ast::expression> parse_string_expr();
		rush::parse_result<ast::expression> parse_integer_expr();
		rush::parse_result<ast::expression> parse_floating_expr();
		rush::parse_result<ast::expression> parse_identifier_expr();
      rush::parse_result<ast::expression> parse_subscript_expr(rush::parse_result<ast::expression>);
      rush::parse_result<ast::expression> parse_member_access_expr(rush::parse_result<ast::expression>);

		rush::parse_result<ast::expression> parse_unary_expr();
		rush::parse_result<ast::expression> parse_unary_postfix_expr(rush::parse_result<ast::expression>);
		rush::parse_result<ast::expression> parse_binary_expr(rush::parse_result<ast::expression>);
		rush::parse_result<ast::expression> parse_binary_expr_rhs();

      rush::parse_result<ast::expression> parse_ternary_expr(rush::parse_result<ast::expression>);
      rush::parse_result<ast::expression> parse_invoke_expr(rush::parse_result<ast::expression>);

      rush::parse_result<ast::expression> parse_argument();
      rush::parse_result<ast::expression_list> parse_argument_list();
      rush::parse_result<ast::expression_list> parse_argument_list(rush::parse_result<ast::expression>);

      rush::parse_result<ast::expression> parse_new_expr();
      rush::parse_result<ast::expression> parse_spread_expr();


      std::unique_ptr<diag::syntax_error> parse_terminator() {
         auto tok = peek_with_indent();
         while (tok.is(symbols::semi_colon)) {
            next_with_indent();
            tok = peek_with_indent();
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
