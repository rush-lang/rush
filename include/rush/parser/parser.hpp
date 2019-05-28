#pragma once

#ifndef RUSH_PARSER_HPP
#define RUSH_PARSER_HPP

#include "fmt/format.h"

#include "rush/core/iterator.hpp"

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/stmts/block.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"

#include "rush/parser/options.hpp"
#include "rush/parser/symbol.hpp"
#include "rush/parser/scope.hpp"
#include "rush/parser/parse.hpp"

#include <optional>

namespace rush {
	class parser {
		using lxa_iterator = lexical_analysis::const_iterator;
		using lxa_iterator_difference_type = std::iterator_traits<lxa_iterator>::difference_type;

	public:
		explicit parser(parser_options const& opts)
			: _opts(opts) {}

		std::unique_ptr<ast::node> parse(lexical_analysis const& lxa) {
			initialize(lxa);

         std::vector<std::unique_ptr<ast::declaration>> decls;
         while (peek_with_indent().is_not(symbols::eof)) {
			   auto decl = parse_toplevel_decl();
            if (!decl) return nullptr;

            if (decl == nullptr) {
               // parse top-level expression.
			      auto expr = parse_expr();
               if (!expr) return nullptr;

            } else {
               decls.push_back(std::move(decl));
            }
         }

         return !decls.empty()
            ? ast::decls::block(std::move(decls))
            : nullptr;
		}

	private:
		static const lexical_token eof;

      rush::scope_chain _scope;
		parser_options _opts;
		std::pair<
			lxa_iterator,
			lxa_iterator> _range;

		void initialize(lexical_analysis const& lxa) {
			_range = {
				lxa.begin(),
				lxa.end()
			};
		}

		std::string format(std::string str) {
			return std::move(str);
		}

		std::string format(lexical_token const& tok) {
			return to_string(tok);
		}

		template <typename... Args>
		std::nullptr_t fatal(std::string msg, Args&&... args) {
			return nullptr;
		}

		template <typename... Args>
		std::nullptr_t error(std::string msg, lexical_token const& tok, Args&&... args) {
			fmt::print("error {}: {}\n",
				to_string(tok.location()),
				fmt::format(msg, to_string(tok), format(std::forward<Args>(args))...));
			return nullptr;
		}

		template <typename... Args>
		std::nullptr_t warn(std::string msg, Args&&... args) {
			return nullptr;
		}

		lexical_token const& peek_with_indent(lxa_iterator_difference_type offset = 0) {
			auto temp = _range.first;
			advance(temp, _range.second, offset);
			return temp != _range.second ? *temp : eof;
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
			return temp != _range.second ? *temp : eof;
		}

		lexical_token const& next_skip_indent() {
			auto* ptok = &next_with_indent();
			for (; ptok->is_any(symbols::indent, symbols::dedent); ptok = &next_with_indent());
			return *ptok;
		}


      // types
      std::optional<ast::type_ref> try_parse_type();
		std::optional<ast::type_ref> try_parse_type_annotation();

		// declarations.
      std::unique_ptr<ast::declaration> parse_toplevel_decl();

		template <typename DeclT>
		std::unique_ptr<DeclT> _parse_storage_decl(std::string,
			std::unique_ptr<DeclT> (*)(std::string, ast::type_ref, std::unique_ptr<ast::expression>));

		std::unique_ptr<ast::constant_declaration> parse_constant_decl();
		std::unique_ptr<ast::variable_declaration> parse_variable_decl();
		std::unique_ptr<ast::function_declaration> parse_function_decl();

      std::unique_ptr<ast::statement> parse_function_body();
      std::unique_ptr<ast::statement> parse_function_expr_body();
      std::unique_ptr<ast::statement> parse_function_stmt_body();

      std::unique_ptr<ast::parameter_list> parse_parameter_list();

		// statements.
      std::unique_ptr<ast::statement> parse_stmt();
		std::unique_ptr<ast::statement> parse_if_stmt();
      std::unique_ptr<ast::statement> parse_else_stmt();
		std::unique_ptr<ast::statement> parse_for_stmt();
		std::unique_ptr<ast::statement> parse_while_stmt();
		std::unique_ptr<ast::statement> parse_return_stmt();
      std::unique_ptr<ast::statement> parse_break_stmt();
      std::unique_ptr<ast::statement> parse_continue_stmt();
      std::unique_ptr<ast::statement> parse_switch_stmt();
      std::unique_ptr<ast::statement> parse_throw_stmt();
      std::unique_ptr<ast::statement> parse_try_stmt();
      std::unique_ptr<ast::statement> parse_yield_stmt();
      std::unique_ptr<ast::statement> parse_with_stmt();

		std::unique_ptr<ast::statement> parse_compound_stmt();
		std::unique_ptr<ast::statement_block> parse_block_stmt();

		// expressions.
		std::unique_ptr<ast::expression> parse_expr();
		std::unique_ptr<ast::expression> parse_paren_expr();

		std::unique_ptr<ast::expression> parse_primary_expr();

		std::unique_ptr<ast::expression> parse_string_expr();
		std::unique_ptr<ast::expression> parse_integer_expr();
		std::unique_ptr<ast::expression> parse_floating_expr();
		std::unique_ptr<ast::expression> parse_identifier_expr();

		std::unique_ptr<ast::unary_expression> parse_unary_expr();
		std::unique_ptr<ast::unary_expression> parse_unary_postfix_expr(std::unique_ptr<ast::expression> op);
		std::unique_ptr<ast::binary_expression> parse_binary_expr(std::unique_ptr<ast::expression> lhs);
		std::unique_ptr<ast::expression> parse_binary_expr_rhs();

		std::unique_ptr<ast::expression> parse_initializer();
	};
}

#endif // RUSH_PARSER_HPP
