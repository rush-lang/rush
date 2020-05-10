#include "rush/lexer/token.hpp"

namespace rush {

	std::string lexical_token::text() const {
		using std::to_string;
		using rush::to_string;

		return std::visit(overloaded {
			[](auto& arg) { return to_string(arg); },
			[](error_t const& arg) { return arg.msg; },
			[](comment_t const& arg) { return arg.comm; },
			[](identifier_t const& arg) { return arg.text; },
			[](std::string arg) { return std::move(arg); },
			[](integral_t const& arg) -> std::string {
				auto str = to_string(arg.val);
				switch (arg.suffix) {
				case lexical_token_suffix::long_literal: return str + "l";
				case lexical_token_suffix::unsigned_literal: return str + "u";
            case lexical_token_suffix::unsigned_long_literal: return str + "ul";
				default: return str;
				}
			},
			[](floating_t const& arg) -> std::string {
				auto str = to_string(arg.val);
				switch (arg.suffix) {
					case lexical_token_suffix::float_literal: return str + "f";
					default: return str;
				}
			},
			[](symbol_token_t const& arg) -> std::string {
				switch (arg) {
				case symbols::eof: return "<eof>";
				case symbols::indent: return "<indent>";
				case symbols::dedent: return "<dedent>";
				default: return to_string(arg);
				}
			},
		}, _val);
	}


	std::string debug_string(lexical_token const& tok) {
		std::ostringstream oss;
      oss << "<" << tok.location() << " ";

		if (tok.is_any(symbols::indent, symbols::dedent, symbols::eof)) {
         oss << to_string(tok) << ">";
		   return oss.str();
      }

      switch (tok.type()) {
      case rush::lexical_token_type::error: oss << "error"; break;
      case rush::lexical_token_type::comment: oss << "comment"; break;
      case rush::lexical_token_type::symbol: oss << "symbol"; break;
      case rush::lexical_token_type::keyword: oss << "keyword"; break;
      case rush::lexical_token_type::identifier: oss << "identifier"; break;
      case rush::lexical_token_type::string_literal: oss << "string"; break;
      case rush::lexical_token_type::integer_literal: oss << "integer"; break;
      case rush::lexical_token_type::floating_literal: oss << "floating"; break;
      }

      oss << " \"" << to_string(tok) << "\">";
      return oss.str();
	}
}
