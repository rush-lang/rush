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
				next_skip_indent(); // consume ':' token.
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

	std::unique_ptr<ast::declaration> parser::parse_constant_decl() {
		assert(peek_skip_indent().is(keywords::let_) && "expected the 'let' keyword.");
		next_skip_indent(); // consume let token
		using function_type = std::unique_ptr<ast::constant_declaration>(*)(
			rush::scope& scope,
			std::string name,
			ast::type type,
			std::unique_ptr<ast::expression> init);
		return _parse_storage_decl("constant", static_cast<function_type>(&decls::constant));
	}

	std::unique_ptr<ast::declaration> parser::parse_variable_decl() {
		assert(peek_skip_indent().is(keywords::var_) && "expected the 'var' keyword.");
		next_skip_indent(); // consume var token
		using function_type = std::unique_ptr<ast::variable_declaration>(*)(
			rush::scope& scope,
			std::string name,
			ast::type type,
			std::unique_ptr<ast::expression> init);
		return _parse_storage_decl("variable", static_cast<function_type>(&decls::variable));
	}
}