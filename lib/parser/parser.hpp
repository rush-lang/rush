#pragma once

#ifndef RUSH_PARSER_HPP
#define RUSH_PARSER_HPP

#include "fmt/format.h"

#include "rush/core/iterator.hpp"

#include "rush/ast/type.hpp"
#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"
#include "rush/ast/statement.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"
#include "rush/parser/parse.hpp"

namespace rush {
	class parser {
		using lxa_iterator = lexical_analysis::const_iterator;
		using lxa_iterator_difference_type = std::iterator_traits<lxa_iterator>::difference_type;

	public:
		explicit parser(parser_options const& opts)
			: _opts(opts) {}

		std::unique_ptr<ast::node> parse(lexical_analysis const& lxa) {
			initialize(lxa);
			return parse_expression();
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

		template <typename... Args>
		std::nullptr_t fatal(std::string msg, Args&&... args) {
			return nullptr;
		}

		template <typename... Args>
		std::nullptr_t error(std::string msg, Args&&... args) {
			fmt::print("error: {}", fmt::format(msg, to_string(std::forward<Args>(args))...));
			return nullptr;
		}

		template <typename... Args>
		std::nullptr_t warn(std::string msg, Args&&... args) {
			return nullptr;
		}

		lexical_token const& peek(lxa_iterator_difference_type offset = 0) {
			auto temp = _range.first;
			advance(temp, _range.second, offset);
			return temp != _range.second ? *temp : eof;
		}

		lexical_token const& peek_skip_indent(lxa_iterator_difference_type offset = 0) {
			auto i = offset;
			auto* ptok = &peek(offset);
			for (; ptok->is_any(symbols::indent, symbols::dedent); ptok = &peek(i + offset), ++i);
			return *ptok;
		}

		lexical_token const& next() {
			auto temp = _range.first;
			advance(_range.first, _range.second, 1);
			return temp != _range.second ? *temp : eof;
		}

		lexical_token const& next_skip_indent() {
			auto* ptok = &next();
			for (; ptok->is_any(symbols::indent, symbols::dedent); ptok = &next());
			return *ptok;
		}

		std::unique_ptr<ast::expression> parse_expression();
		std::unique_ptr<ast::expression> parse_parenthesised_expression();

		std::unique_ptr<ast::expression> parse_primary_expression();
		std::unique_ptr<ast::expression> parse_string_expression();
		std::unique_ptr<ast::expression> parse_integer_expression();
		std::unique_ptr<ast::expression> parse_floating_expression();
		std::unique_ptr<ast::expression> parse_identifier_expression();
	};
}

#endif // RUSH_PARSER_HPP
