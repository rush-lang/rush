#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"
#include "rush/lexer/analysis.hpp"

#include "rush/parser/parser.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>


using namespace rush;

namespace tok = rush::tokens;
namespace sym = rush::symbols;
namespace kws = rush::keywords;

using ltt = rush::lexical_token_type;


class parser {
public:
	parser(parser_options const& opts)
		: _options(opts) {}

	std::unique_ptr<ast::node> parse(lexical_analysis const& lxa) {
		initialize(lxa);

		return nullptr;
	}

private:
	parser_options _options;
	std::pair<
		lexical_analysis::const_iterator,
		lexical_analysis::const_iterator> _range;

	void initialize(lexical_analysis const& lxa) {
		_range = {
			lxa.begin(),
			lxa.end()
		};
	}

	lexical_token const& peek() {
		return *_range.first;
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

	template <typename... Args>
	void info(std::string msg, Args&&... args) {

	}

	std::unique_ptr<ast::literal_expression> parse_literal_expression() {
		return nullptr;
	}

	std::unique_ptr<ast::binary_expression> parse_binary_expression() {
		return nullptr;
	}

	std::unique_ptr<ast::unary_expression> parse_unary_expression() {
		return nullptr;
	}
};


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