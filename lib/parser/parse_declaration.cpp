#include "parser.hpp"

namespace rush {

	// std::unique_ptr<ast::declaration> parser::parse_declaration() {
	// 	auto& tok = next_skip_indent();

	// 	if (tok.is(keywords::let_)) {
	// 		return nullptr;
	// 		// return parse_constant_declaration();
	// 	}

	// 	if (tok.is(keywords::var_)) {
	// 		return nullptr;
	// 		// return parse_variable_declaration();
	// 	}

	// 	return error("expected declaration, but found \'{}\'", tok);
	// }
}