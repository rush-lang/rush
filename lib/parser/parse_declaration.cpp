#include "rush/ast/decl/constant.hpp"
#include "rush/ast/decl/variable.hpp"
#include "parser.hpp"

namespace rush {
	namespace decls = ast::decls;

	std::unique_ptr<ast::expression> parser::parse_initializer() {
		return parse_expr();
	}

	template <typename DeclT>
	std::unique_ptr<DeclT> parser::_parse_storage_decl(std::string storage_type,
		std::unique_ptr<DeclT> (*fptr)(rush::scope&, std::string, ast::type, std::unique_ptr<ast::expression>)
	) {
		if (peek_skip_indent().is(symbols::left_bracket)) {
			// parse_destructure_pattern.
			return error("destructuring is currently not supported.", peek_skip_indent());
		}

		if (peek_skip_indent().is_identifier()) {
			auto ident = next_skip_indent();
			std::optional<ast::type> type;

			if (peek_skip_indent().is(symbols::colon)) {
				type = parse_type_annotation();
			}

			if (!peek_skip_indent().is(symbols::equals)) {
				return error("expected initializer for {1} '{2}', before '{0}'.", peek_skip_indent(), storage_type, ident);
			}

			next_skip_indent(); // consume '=' token.
			auto init = parse_initializer();
			if (!init) return nullptr;

			return type != std::nullopt
				? (*fptr)(_scope, ident.text(), *type, std::move(init))
				: (*fptr)(_scope, ident.text(), init->result_type(), std::move(init));
		}

		return error("expected an identifier before '{}'.", next_skip_indent());
	}

	std::unique_ptr<ast::constant_declaration> parser::parse_constant_decl() {
		assert(peek_skip_indent().is(keywords::let_) && "expected the 'let' keyword.");
		next_skip_indent(); // consume let token
		using function_type = std::unique_ptr<ast::constant_declaration>(*)(
			rush::scope& scope,
			std::string name,
			ast::type type,
			std::unique_ptr<ast::expression> init);
		return _parse_storage_decl("constant", static_cast<function_type>(&decls::constant));
	}

	std::unique_ptr<ast::variable_declaration> parser::parse_variable_decl() {
		assert(peek_skip_indent().is(keywords::var_) && "expected the 'var' keyword.");
		next_skip_indent(); // consume var token
		using function_type = std::unique_ptr<ast::variable_declaration>(*)(
			rush::scope& scope,
			std::string name,
			ast::type type,
			std::unique_ptr<ast::expression> init);
		return _parse_storage_decl("variable", static_cast<function_type>(&decls::variable));
	}

	// std::unique_ptr<ast::parameter_list> parser::parse_parameter_list() {
	// 	assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected an opening parenthesis '('");
	// 	next_skip_indent(); // consume '(' symbol

	// 	auto pset = parse_parameter_set();

	// 	if (peek_)
	// }

	std::unique_ptr<ast::function_declaration> parser::parse_function_decl() {
		// assert(peek_skip_indent().is(keywords::func_) && "expected the 'func' keyword.");
		// next_skip_indent(); // consume func keyword.

		// if (!peek_skip_indent().is_identifier()) {
		// 	return error("expected an identifier for the function name before '{}'.", next_skip_indent());
		// auto ident = next_skip_indent();

		// _scope = _scope.push_scope();

		// if (peek_skip_indent().is_not(symbols::left_parenthesis))
		// 	return error("expected '(' before '{}'.", next_skip_indent());
		// auto plist = parse_parameter_list();
		// if (!plist) return nullptr;

		// std::unique_ptr<statement> body;
		// std::optional<ast::type> rtype;

		// if (peek_skip_indent().is(symbols::arrow)) {
		// 	auto expr = parse_expr();
		// 	if (!expr) return nullptr;
		// 	body = stmts::return_(expr);
		// } else if (peek_skip_indent().is(symbols::colon)) {
		// 	if (peek_with_indent().is_not(symbols::indent)) {
		// 		rtype = parse_type_annotation();
		// 		if (!rtype) return nullptr;
		// 	}

		// 	if (peek_with_indent().is(symbols::indent)) {
		// 		body = parse_block_stmt();
		// 		if (!body) return nullptr;
		// 	}
		// } else {
		// 	return error("expected function body before '{}'", next_with_indent());
		// }

		// _scope = _scope.parent();
		// return decls::function(_scope,
		// 	ident.text(), plist,
		// 	stmts::return_(body));
		return nullptr;
	}
}