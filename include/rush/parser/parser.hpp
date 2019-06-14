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

#include <optional>

namespace rush {
	class parser {
		using lxa_iterator = lexical_analysis::const_iterator;
		using lxa_iterator_difference_type = std::iterator_traits<lxa_iterator>::difference_type;

	public:
		explicit parser(parser_options const& opts)
			: _opts(opts)
         , _eof(tokens::eof()) {}

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

      std::unique_ptr<ast::module> _module;
      std::unique_ptr<ast::context> _context;
		std::pair<lxa_iterator, lxa_iterator> _range;


		void initialize(lexical_analysis const& lxa) {
         auto loc = lxa.back().location();
         _eof = tokens::eof(loc.next_column(lxa.back().size()));
			_range = { lxa.begin(), lxa.end() };
         _context = std::make_unique<ast::context>();
         _module = std::make_unique<ast::module>(std::string { lxa.id() });
		}

		lexical_token const& peek_with_indent(lxa_iterator_difference_type offset = 0) {
			auto temp = _range.first;
			advance(temp, _range.second, offset);
			return temp != _range.second ? *temp : _eof;
		}

		lexical_token const& peek_skip_indent(lxa_iterator_difference_type offset = 0) {
			auto i = offset;
			auto* ptok = &peek_with_indent(offset);
			for (; ptok->is_any(symbols::indent, symbols::dedent); ptok = &peek_with_indent(i + offset), ++i);
			return *ptok;
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

		lexical_token const& next_with_indent() {
			auto temp = _range.first;
			advance(_range.first, _range.second, 1);
			return temp != _range.second ? *temp : _eof;
		}

		lexical_token const& next_skip_indent() {
			auto* ptok = &next_with_indent();
			for (; ptok->is_any(symbols::indent, symbols::dedent); ptok = &next_with_indent());
			return *ptok;
		}

      // modules.
      rush::parse_result<ast::module> parse_module();
      rush::parse_result<ast::import_declaration> parse_import_decl();

      // types.
      rush::parse_type_result parse_type();
      rush::parse_type_result parse_simple_type();
		rush::parse_type_result parse_type_annotation();

      rush::parse_type_result parse_array_type(ast::type_ref);

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

		rush::parse_result<ast::expression> parse_string_expr();
		rush::parse_result<ast::expression> parse_integer_expr();
		rush::parse_result<ast::expression> parse_floating_expr();
		rush::parse_result<ast::expression> parse_identifier_expr();

		rush::parse_result<ast::expression> parse_unary_expr();
		rush::parse_result<ast::expression> parse_unary_postfix_expr(rush::parse_result<ast::expression> op);
		rush::parse_result<ast::expression> parse_binary_expr(rush::parse_result<ast::expression> lhs);
		rush::parse_result<ast::expression> parse_binary_expr_rhs();

      rush::parse_result<ast::expression> parse_ternary_expr(rush::parse_result<ast::expression> cond);
      rush::parse_result<ast::expression> parse_invoke_expr(rush::parse_result<ast::expression> expr);
      rush::parse_result<ast::argument_list> parse_argument_list();

      rush::parse_result<ast::declaration> scope_insert(std::unique_ptr<ast::declaration> decl, rush::lexical_token const& ident);

      template <typename NodeT>
      rush::parse_result<NodeT> terminated(rush::parse_result<NodeT>(parser::*parse_fn)()) {
         auto result = (this->*parse_fn)();
         if (result.failed())
            return std::move(result);

         auto tok = peek_with_indent();
         if (tok.is_not(symbols::semi_colon))
            return diag::errs::expected(tok, ";");

         while (tok.is(symbols::semi_colon)) {
            next_with_indent();
            tok = peek_with_indent();
         }

         return std::move(result);
      }
	};
}

#endif // RUSH_PARSER_HPP
