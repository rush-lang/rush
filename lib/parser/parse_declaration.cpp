#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"
#include "rush/ast/stmts/control.hpp"

#include "rush/parser/parser.hpp"

namespace rush {
	namespace decls = ast::decls;
   namespace stmts = ast::stmts;


   std::unique_ptr<ast::declaration> parser::parse_toplevel_decl() {
		auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         switch (tok.keyword()) {
         case keywords::let_: return parse_constant_decl();
         case keywords::var_: return parse_variable_decl();
         case keywords::func_: return parse_function_decl();
         default: return error("unsupported '{}'", tok);
         }
      }

      return error("unsupported '{}'", tok);
   }

	template <typename DeclT>
	std::unique_ptr<DeclT> parser::_parse_storage_decl(std::string storage_type,
		std::unique_ptr<DeclT> (*fptr)(std::string, ast::type_ref, std::unique_ptr<ast::expression>)
	) {
		if (peek_skip_indent().is(symbols::left_bracket)) {
			// parse_destructure_pattern.
			return error("destructuring is currently not supported.", peek_skip_indent());
		}

		if (peek_skip_indent().is_identifier()) {
			auto ident = next_skip_indent();
			std::optional<ast::type_ref> type;

			if (peek_skip_indent().is(symbols::colon)) {
				type = try_parse_type_annotation();
			}

			if (!peek_skip_indent().is(symbols::equals)) {
				return error("expected initializer for {1} '{2}', before '{0}'.", peek_skip_indent(), storage_type, ident);
			}

			next_skip_indent(); // consume '=' token.
			auto init = parse_initializer();
			if (!init) return nullptr;

			auto decl = type != std::nullopt
				? (*fptr)(ident.text(), *type, std::move(init))
				: (*fptr)(ident.text(), init->result_type(), std::move(init));

         _scope.insert({ *decl });
         return std::move(decl);
		}

		return error("expected an identifier before '{}'.", next_skip_indent());
	}

	std::unique_ptr<ast::constant_declaration> parser::parse_constant_decl() {
		assert(peek_skip_indent().is(keywords::let_) && "expected the 'let' keyword.");
		next_skip_indent(); // consume let token
		using function_type = std::unique_ptr<ast::constant_declaration>(*)(
			std::string name,
			ast::type_ref type,
			std::unique_ptr<ast::expression> init);
		return _parse_storage_decl("constant", static_cast<function_type>(&decls::constant));
	}

	std::unique_ptr<ast::variable_declaration> parser::parse_variable_decl() {
		assert(peek_skip_indent().is(keywords::var_) && "expected the 'var' keyword.");
		next_skip_indent(); // consume var token
		using function_type = std::unique_ptr<ast::variable_declaration>(*)(
			std::string name,
			ast::type_ref type,
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
		assert(peek_skip_indent().is(keywords::func_) && "expected the 'func' keyword.");
		next_skip_indent(); // consume func keyword.

		if (!peek_skip_indent().is_identifier())
			return error("expected an identifier for the function name before '{}'.", next_skip_indent());
		auto ident = next_skip_indent();

		if (peek_skip_indent().is_not(symbols::left_parenthesis))
		 	return error("expected '(' before '{}'.", next_skip_indent());
      next_skip_indent(); // consume left parenthesis

		// auto plist = parse_parameter_list();
		// if (!plist) return nullptr;

      if (peek_skip_indent().is_not(symbols::right_parenthesis))
         return error("expected '(' before '{}'.", next_skip_indent());
      next_skip_indent();

      // auto type = try_parse_type();
		auto body = parse_function_body();
		return decls::function(ident.text(), std::move(body));
	}

   std::unique_ptr<ast::statement> parser::parse_function_body() {
      _scope.push(rush::scope_kind::function);

      auto body = std::unique_ptr<ast::statement> {};
      if (peek_skip_indent().is(symbols::arrow))
         body = parse_function_expr_body();
      else if (peek_skip_indent().is(symbols::colon))
         body = parse_function_stmt_body();

      _scope.pop();
      return body == nullptr
         ? error("expected function definition before '{}'", next_skip_indent())
         : std::move(body);
   }

   std::unique_ptr<ast::statement> parser::parse_function_expr_body() {
		assert(peek_skip_indent().is(symbols::arrow) && "expected an '=>' symbol.");
      next_skip_indent();
      return stmts::return_(parse_expr());
   }

   std::unique_ptr<ast::statement> parser::parse_function_stmt_body() {
		assert(peek_skip_indent().is(symbols::colon) && "expected a ':' symbol.");
      next_skip_indent();

      if (peek_with_indent().is_not(symbols::indent)) {
         return error("expected function body before '{}'", next_with_indent());
      }

      // todo: pass the kind of statement block.
      return parse_block_stmt();
   }
}
