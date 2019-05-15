#pragma once

#ifndef RUSH_PARSER_HPP
#define RUSH_PARSER_HPP

#include "fmt/format.h"

#include "rush/core/iterator.hpp"

#include "rush/ast/types/type.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"
#include "rush/parser/options.hpp"
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
			auto tok = peek_skip_indent();

			if (tok.is_keyword()) {
				switch (tok.keyword()) {
				case keywords::let_: return parse_constant_decl();
				case keywords::var_: return parse_variable_decl();
				case keywords::func_: return parse_function_decl();
				default: break;
				}
			}

			// parse top-level expression.
			return parse_expr();
		}

	private:
		static const lexical_token eof;

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

		std::optional<ast::type> parse_type_annotation() {
			assert(peek_skip_indent().is(symbols::colon) && "expected a type annotation symbol ':'");
			next_skip_indent();

			auto tok = next_skip_indent();
			if (!tok.is_keyword() && !tok.is_identifier()) {
				error("expected a type identifier before '{}'", tok);
				return std::nullopt;
			}

			// auto sym = _scope.lookup(tok.text());
			// if (!sym.is_type()) {
			// 	error("symbol '{}' does not name a type.", tok);
			// 	return std::nullopt;
			// }

			return ast::types::error_type;
		}

		std::unique_ptr<ast::expression> parse_initializer();


		// declarations.
		template <typename DeclT>
		std::unique_ptr<DeclT> _parse_storage_decl(std::string,
			std::unique_ptr<DeclT> (*)(std::string, ast::type, std::unique_ptr<ast::expression>));

		std::unique_ptr<ast::constant_declaration> parse_constant_decl();
		std::unique_ptr<ast::variable_declaration> parse_variable_decl();
		std::unique_ptr<ast::function_declaration> parse_function_decl();

		// statements.
		std::unique_ptr<ast::statement> parse_if_stmt();
		std::unique_ptr<ast::statement> parse_for_stmt();
		std::unique_ptr<ast::statement> parse_while_stmt();
		std::unique_ptr<ast::statement> parse_return_stmt();

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
		std::unique_ptr<ast::binary_expression> parse_binary_expr(std::unique_ptr<ast::expression> lhs);
		std::unique_ptr<ast::expression> parse_binary_expr_rhs();
	};
}

#endif // RUSH_PARSER_HPP
