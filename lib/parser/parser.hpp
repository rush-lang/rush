#pragma once

#ifndef RUSH_PARSER_HPP
#define RUSH_PARSER_HPP

#include "fmt/format.h"

#include "rush/core/iterator.hpp"

#include "rush/sema/scope.hpp"

#include "rush/ast/type.hpp"
#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"
#include "rush/ast/statement.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"
#include "rush/parser/parse.hpp"

#include <optional>

namespace rush {
	class parser {
		using lxa_iterator = lexical_analysis::const_iterator;
		using lxa_iterator_difference_type = std::iterator_traits<lxa_iterator>::difference_type;

	public:
		explicit parser(parser_options const& opts)
			: _opts(opts)
			, _scope(*opts.scope) {}

		std::unique_ptr<ast::node> parse(lexical_analysis const& lxa) {
			initialize(lxa);
			auto tok = peek_skip_indent();

			if (tok.is_keyword()) {
				switch (tok.keyword()) {
				case keywords::let_: return parse_constant_declaration();
				case keywords::var_: return parse_variable_declaration();
				default: break;
				}
			}

			// parse top-level expression.
			return parse_expression();
		}

	private:
		static const lexical_token eof;

		scope& _scope;
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
		std::nullptr_t error(std::string msg, Args&&... args) {
			fmt::print("error: {}", fmt::format(msg, format(std::forward<Args>(args))...));
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

		std::optional<ast::type> parse_type() {
			return { ast::int_type };
		}

		std::unique_ptr<ast::expression> parse_initializer();


		// declarations.
		template <typename DeclT>
		std::unique_ptr<DeclT> _parse_storage_declaration(std::string,
			std::unique_ptr<DeclT> (*)(rush::scope&, std::string, ast::type, std::unique_ptr<ast::expression>));

		std::unique_ptr<ast::declaration> parse_constant_declaration();
		std::unique_ptr<ast::declaration> parse_variable_declaration();

		std::unique_ptr<ast::function_declaration> parse_function_declaration();

		// expressions.
		std::unique_ptr<ast::expression> parse_expression();
		std::unique_ptr<ast::expression> parse_parenthesised_expression();

		std::unique_ptr<ast::expression> parse_primary_expression();

		std::unique_ptr<ast::expression> parse_string_expression();
		std::unique_ptr<ast::expression> parse_integer_expression();
		std::unique_ptr<ast::expression> parse_floating_expression();
		std::unique_ptr<ast::expression> parse_identifier_expression();

		std::unique_ptr<ast::unary_expression> parse_unary_expression();
		std::unique_ptr<ast::binary_expression> parse_binary_expression(std::unique_ptr<ast::expression> lhs);

		std::unique_ptr<ast::expression> parse_binary_expression_rhs();
		std::unique_ptr<ast::binary_expression> parse_multiplication_expression(std::unique_ptr<ast::expression> lhs);
		std::unique_ptr<ast::binary_expression> parse_division_expression(std::unique_ptr<ast::expression> lhs);
		std::unique_ptr<ast::binary_expression> parse_subtraction_expression(std::unique_ptr<ast::expression> lhs);
		std::unique_ptr<ast::binary_expression> parse_addition_expression(std::unique_ptr<ast::expression> lhs);
	};
}

#endif // RUSH_PARSER_HPP
