#include "rush/core/iterator.hpp"

#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"
#include "rush/lexer/analysis.hpp"

#include "rush/ast/type.hpp"
#include "rush/ast/expr/unary.hpp"
#include "rush/ast/expr/binary.hpp"
#include "rush/ast/expr/literal.hpp"
#include "rush/ast/expr/identifier.hpp"

#include "rush/parser/parser.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>


using namespace rush;
namespace sy = rush::symbols;
namespace kw = rush::keywords;

using ltt = rush::lexical_token_type;


class parser {
	using lxa_iterator = lexical_analysis::const_iterator;
	using lxa_iterator_difference_type = std::iterator_traits<lxa_iterator>::difference_type;

public:
	parser(parser_options const& opts)
		: _options(opts) {}

	std::unique_ptr<ast::node> parse(lexical_analysis const& lxa) {
		initialize(lxa);

		return nullptr;
	}

private:
	static const lexical_token eof;

	parser_options _options;
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
	void fatal(std::string msg, Args&&... args) {
	}

	template <typename... Args>
	void error(std::string msg, Args&&... args) {
	}

	template <typename... Args>
	void warn(std::string msg, Args&&... args) {
	}

	lexical_token const& peek(lxa_iterator_difference_type offset = 0) {
		auto temp = _range.first;
		advance(temp, _range.second, offset);
		return temp != _range.second ? *temp : eof;
	}

	lexical_token const& peek_skip_indent(lxa_iterator_difference_type offset = 0) {
		auto* ptok = &peek(offset);
		for (decltype(offset) i = offset; ptok->is_any(sy::indent, sy::dedent); ptok = &peek(i + offset), ++i);
		return *ptok;
	}

	lexical_token const& next_skip_indent() {
		auto* ptok = &next();
		for (; ptok->is_any(sy::indent, sy::dedent); ptok = &next());
		return *ptok;
	}

	lexical_token const& next() {
		if (advance(_range.first, _range.second, 1)) return *_range.first;
		return eof;
	}

	ast::type const& parse_type() {
		auto& curtok = peek_skip_indent();
		if (curtok.is_keyword()) {
			switch (curtok.keyword()) {
			case kw::void_: return ast::void_type;
			case kw::bool_: return ast::bool_type;
			case kw::sbyte_: return ast::sbyte_type;
			case kw::byte_: return ast::byte_type;
			case kw::int_: return ast::int_type;
			case kw::uint_: return ast::uint_type;
			case kw::long_: return ast::long_type;
			case kw::ulong_: return ast::ulong_type;
			case kw::float_: return ast::float_type;
			case kw::double_: return ast::double_type;
			case kw::string_: return ast::string_type;
			default: break;
			};
		}
		else if (curtok.is_identifier()) {
			// todo: find entry in the symbol table, if it doesn't
			// exist, no matter, the entry may be filled later
			// in the program.
		}

		error("expected a type, but found '{}'", curtok);
		return ast::none_type;
	}

	std::unique_ptr<ast::literal_expression> parse_literal_expression() {
		auto& curtok = peek_skip_indent();

		if (curtok.is(kw::true_)) return ast::literal_expr(true);
		if (curtok.is(kw::false_)) return ast::literal_expr(false);
		if (curtok.is_string_literal()) return ast::literal_expr(curtok.text());

		if (curtok.is_integer_literal()) {
			switch (curtok.suffix()) {
			case lexical_token_suffix::none: return ast::literal_expr(curtok.integer_value(), ast::int_type);
			case lexical_token_suffix::long_literal: return ast::literal_expr(curtok.integer_value(), ast::long_type);
			case lexical_token_suffix::unsigned_literal: return ast::literal_expr(curtok.integer_value(), ast::uint_type);
			// case lexical_token_suffix::unsigned_long_literal: return ast::literal_expr(curtok.integer(), ast::ulong_type);
			default: throw;
			}
		}

		if (curtok.is_floating_literal()) {
			switch (curtok.suffix()) {
			case lexical_token_suffix::none: return ast::literal_expr(curtok.floating_value(), ast::double_type);
			case lexical_token_suffix::float_literal: return ast::literal_expr(curtok.floating_value(), ast::float_type);
			default: throw;
			}
		}

		error("expected literal expression but found '{}'", curtok);
		return nullptr;
	}

	std::unique_ptr<ast::binary_expression> parse_binary_expression() {
		error("expected binary expression but found '{}'", nullptr);
		return nullptr;
	}

	std::unique_ptr<ast::unary_expression> parse_unary_expression() {
		error("expected unary expression but found '{}'", nullptr);
		return nullptr;
	}

	std::unique_ptr<ast::expression> parse_primary_expression() {
		auto curtok = peek();
		switch (curtok.type()) {
		default:
			error("unknown token '{}' when parsing expression.", curtok);
		case lexical_token_type::error:
			error(curtok.text());
			return nullptr;
		case lexical_token_type::identifier:
			return nullptr;
		case lexical_token_type::string_literal:
		case lexical_token_type::integer_literal:
		case lexical_token_type::floating_literal:
			return parse_literal_expression();
		}
	}
};

const lexical_token parser::eof = tokens::eof();

namespace rush {

	abstract_syntax_tree parse(lexical_analysis const& lxa, parser_options const& opts) {
		auto p = parser { opts };
		auto root = p.parse(lxa);

		return root != nullptr
			? abstract_syntax_tree { std::move(root) }
			: abstract_syntax_tree {};
	}

	abstract_syntax_tree parse(std::string input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}

	abstract_syntax_tree parse(std::istream& input, parser_options const& opts) {
		auto lxa = rush::lex(input);
		return parse(lxa, opts);
	}
} // rush