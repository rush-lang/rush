#include "rush/ast/declarations.hpp"
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
				type = parse_type_annotation();
            if (type == std::nullopt) {
               return error("expected type annotation before '{}'", peek_skip_indent());
            }
			}

			if (!peek_skip_indent().is(symbols::equals)) {
				return error("expected initializer for {1} '{2}', before '{0}'.", peek_skip_indent(), storage_type, ident);
			}

			auto init = parse_initializer();
			if (!init) return nullptr;

			auto decl = type != std::nullopt
				? (*fptr)(ident.text(), *type, std::move(init))
				: (*fptr)(ident.text(), init->result_type(), std::move(init));
         if (!decl) return nullptr;

         if (!_scope.insert({ *decl })) {
            auto existing_decl = _scope.current().lookup_local(ident.text());
            switch (existing_decl.declaration()->kind()) {
            default: assert("unreachable");
            case ast::declaration_kind::constant: return error("local constant named '{}' is already defined in this scope.", ident);
            case ast::declaration_kind::variable: return error("local variable named '{}' is already defined in this scope.", ident);
            }
         }

         return std::move(decl);
		}

		return error("expected an identifier before '{}'.", next_skip_indent());
	}

	std::unique_ptr<ast::declaration> parser::parse_constant_decl() {
		assert(peek_skip_indent().is(keywords::let_) && "expected the 'let' keyword.");
		next_skip_indent(); // consume let token
		using function_type = std::unique_ptr<ast::constant_declaration>(*)(
			std::string name,
			ast::type_ref type,
			std::unique_ptr<ast::expression> init);
		return _parse_storage_decl("constant", static_cast<function_type>(&decls::constant));
	}

	std::unique_ptr<ast::declaration> parser::parse_variable_decl() {
		assert(peek_skip_indent().is(keywords::var_) && "expected the 'var' keyword.");
		next_skip_indent(); // consume var token
		using function_type = std::unique_ptr<ast::variable_declaration>(*)(
			std::string name,
			ast::type_ref type,
			std::unique_ptr<ast::expression> init);
		return _parse_storage_decl("variable", static_cast<function_type>(&decls::variable));
	}

	std::unique_ptr<ast::parameter_list> parser::parse_parameter_list() {
		assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected an opening parenthesis '('");
		next_skip_indent(); // consume '(' symbol.

      if (peek_skip_indent().is(symbols::right_parenthesis)) {
         next_skip_indent(); // consume ')' symbol.
         return decls::param_list(); // empty parameter list case.
      }

      std::vector<std::unique_ptr<ast::parameter>> ps;
      do {
         auto names = std::vector<rush::lexical_token> {};
         while (peek_skip_indent().is_identifier()) {
            names.emplace_back(next_skip_indent());
            if (peek_skip_indent().is(symbols::comma))
               next_skip_indent(); // skip comma to consume another name.
         }

         if (names.empty())
            return error("expected parameter identifier before '{}'.", next_skip_indent());

         if (peek_skip_indent().is_not(symbols::colon))
            return error("expected type annotation for parameters.", next_skip_indent());

         auto type = parse_type_annotation();
         if (type == std::nullopt) return nullptr;

         std::transform(
            std::make_move_iterator(names.begin()),
            std::make_move_iterator(names.end()),
            std::back_inserter(ps), [this, &type](auto n) {
               auto p = decls::param(n.text(), *type);
               return !_scope.insert({ *p })
                  ? error("redefinition of parameter '{}'.", n)
                  : std::move(p);
         });

      } while (consume_skip_indent(symbols::comma));

      // check for any parameter parsing failures.
      if (std::any_of(ps.begin(), ps.end(), [](auto& p) { return p == nullptr; }))
         return nullptr;

      if (peek_skip_indent().is_not(symbols::right_parenthesis))
         return error("expected ')' before '{}'.", next_skip_indent());
      next_skip_indent();

      return decls::param_list(std::move(ps));
	}


	std::unique_ptr<ast::declaration> parser::parse_function_decl() {
		assert(peek_skip_indent().is(keywords::func_) && "expected the 'func' keyword.");
		next_skip_indent(); // consume func keyword.
      _scope.push(rush::scope_kind::function);

		if (!peek_skip_indent().is_identifier())
			return error("expected an identifier for the function name before '{}'.", next_skip_indent());
		auto ident = next_skip_indent();

		if (peek_skip_indent().is_not(symbols::left_parenthesis))
		 	return error("expected '(' before '{}'.", next_skip_indent());

		auto plist = parse_parameter_list();
		if (!plist) return nullptr;

      std::optional<ast::type_ref> type = {};
      if (peek_skip_indent().is_not(symbols::arrow)
      &&  peek_skip_indent().is_not(symbols::colon))
         type = parse_type();

		auto body = parse_function_body();
      if (!body) return nullptr;

		auto decl = std::nullopt != type
         ? decls::function(ident.text(), *type, std::move(plist), std::move(body))
         : decls::function(ident.text(), std::move(plist), std::move(body));

      _scope.pop();
      if (!_scope.insert({ *decl })) {
         return error("the current context already contains a definition for '{}'.", ident);
      }

      return std::move(decl);
	}

   std::unique_ptr<ast::statement> parser::parse_function_body() {
      if (peek_skip_indent().is(symbols::arrow))
         return parse_function_expr_body();
      if (peek_skip_indent().is(symbols::colon))
         return parse_function_stmt_body();

      return nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_function_expr_body() {
		assert(peek_skip_indent().is(symbols::arrow) && "expected an '=>' symbol.");
      next_skip_indent();

      std::unique_ptr<ast::statement> stmt;
      if (peek_skip_indent().is(keywords::pass_)) {
         next_skip_indent();
         stmt = stmts::pass();
      } else {
         auto expr = parse_expr();
         if (!expr) return nullptr;
         stmt = stmts::return_(std::move(expr));
      }

      std::vector<std::unique_ptr<ast::statement>> stmts;
      stmts.emplace_back(std::move(stmt));
      return stmts::block(std::move(stmts));
   }

   std::unique_ptr<ast::statement> parser::parse_function_stmt_body() {
		assert(peek_skip_indent().is(symbols::colon) && "expected a ':' symbol.");
      next_skip_indent();

      if (peek_with_indent().is_not(symbols::indent)) {
         return error("expected start of function body '<indent>' before '{}'", next_with_indent());
      }

      return parse_block_stmt();
   }
}
