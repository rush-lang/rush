#include "rush/ast/declarations.hpp"
#include "rush/ast/stmts/simple.hpp"
#include "rush/ast/stmts/result.hpp"

#include "rush/parser/parser.hpp"

namespace rush {
	namespace decls = ast::decls;
   namespace stmts = ast::stmts;

   rush::parse_result<ast::declaration> parser::parse_toplevel_decl() {
		auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         switch (tok.keyword()) {
         case keywords::let_: return terminated(&parser::parse_constant_decl);
         case keywords::var_: return terminated(&parser::parse_variable_decl);
         case keywords::func_: return parse_function_decl();
         default: break;
         }
      }

      return error("expected a type, function, or variable declaration", tok);
   }

	rush::parse_result<ast::declaration> parser::_parse_storage_decl(std::string storage_type,
		rush::function_ref<std::unique_ptr<ast::declaration>(std::string, ast::type_ref, std::unique_ptr<ast::expression>)> fn
	) {
		if (peek_skip_indent().is(symbols::left_bracket)) {
			// parse_destructure_pattern.
			return error("destructuring is currently not supported.", peek_skip_indent());
		}

      auto ident = peek_skip_indent();
		if (!ident.is_identifier())
   		return error("expected an identifier before '{}'.", next_skip_indent());
      next_skip_indent();

      auto type = std::optional<ast::type_ref> {};
      if (peek_skip_indent().is(symbols::colon)) {
         type = parse_type_annotation();
         if (type == std::nullopt) {
            return error("expected type annotation before '{}'", peek_skip_indent());
         }
      }

      auto init = rush::parse_result<ast::expression> {};
      if (peek_skip_indent().is(symbols::equals)) {
         next_skip_indent();
         if ((init = parse_expr()).failed())
            return std::move(init).as<ast::declaration>();
      } else if (type == std::nullopt) {
         return error("{1} declaration '{0}' requires either a type-annotation or intializer.", ident, storage_type);
      }

      type = (type == std::nullopt) ? ast::types::undefined : type;
      auto decl = fn(ident.text(), *type, std::move(init));
      if (!decl) return std::move(decl); // todo: should throw here an error here. only reason this should fail is bad-mem-alloc.

      // return scope_insert(std::move(decl), ident);
      if (!_scope.insert({ *decl })) {
         error("local {1} named '{0}' is already defined in this scope.", ident, storage_type);
         // return error(std::move(decl), "local {1} named '{0}' is already defined in this scope.", ident, storage);
      }

      return std::move(decl);
	}

	rush::parse_result<ast::declaration> parser::parse_constant_decl() {
		assert(peek_skip_indent().is(keywords::let_) && "expected the 'let' keyword.");
		next_skip_indent(); // consume let token
		return _parse_storage_decl("constant", [](
         std::string name,
         ast::type_ref type,
         std::unique_ptr<ast::expression> init) {
            return decls::constant(std::move(name), std::move(type), std::move(init));
         });
	}

	rush::parse_result<ast::declaration> parser::parse_variable_decl() {
		assert(peek_skip_indent().is(keywords::var_) && "expected the 'var' keyword.");
		next_skip_indent(); // consume var token
		return _parse_storage_decl("variable", [](
         std::string name,
         ast::type_ref type,
         std::unique_ptr<ast::expression> init) {
            return decls::variable(std::move(name), std::move(type), std::move(init));
         });
	}

	rush::parse_result<ast::parameter_list> parser::parse_parameter_list() {
		assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected an opening parenthesis '('");
		next_skip_indent(); // consume '(' symbol.

      if (peek_skip_indent().is(symbols::right_parenthesis)) {
         next_skip_indent(); // consume ')' symbol.
         return decls::param_list(); // empty parameter list case.
      }

      std::vector<rush::parse_result<ast::parameter>> results;
      do {
         auto names = std::vector<rush::lexical_token> {};
         while (peek_skip_indent().is_identifier()) {
            names.push_back(next_skip_indent());
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
            std::back_inserter(results), [this, &type](auto n) {
               auto p = decls::param(n.text(), *type);
               return !_scope.insert({ *p })
                  ? error("redefinition of parameter '{}'.", n)
                  : rush::parse_result<ast::parameter> { std::move(p) };
         });

      } while (consume_skip_indent(symbols::comma));

      // check for any parameter parsing failures.
      if (std::any_of(results.begin(), results.end(), [](auto& p) { return p.failed(); }))
         return nullptr; // todo: accumulate parameter errors and return.

      if (peek_skip_indent().is_not(symbols::right_parenthesis))
         return error("expected ')' before '{}'.", next_skip_indent());
      next_skip_indent();

      auto params = std::vector<std::unique_ptr<ast::parameter>> {};
      std::move(results.begin(), results.end(), std::back_inserter(params));
      return { decls::param_list(std::move(params)) };
	}


	rush::parse_result<ast::declaration> parser::parse_function_decl() {
		assert(peek_skip_indent().is(keywords::func_) && "expected the 'func' keyword.");
		next_skip_indent(); // consume func keyword.
      _scope.push(rush::scope_kind::function);

		if (!peek_skip_indent().is_identifier())
			return error("expected an identifier for the function name before '{}'.", next_skip_indent());
		auto ident = next_skip_indent();

		if (peek_skip_indent().is_not(symbols::left_parenthesis))
		 	return error("expected '(' before '{}'.", next_skip_indent());

		auto plist_result = parse_parameter_list();
		if (plist_result.failed())
         return std::move(plist_result).as<ast::declaration>();

      std::optional<ast::type_ref> type = {};
      if (peek_skip_indent().is_not(symbols::arrow)
      &&  peek_skip_indent().is_not(symbols::colon))
         type = parse_type();

		auto body_result = parse_function_body();
      if (body_result.failed())
         return std::move(body_result).as<ast::declaration>();

		auto decl = std::nullopt != type
         ? decls::function(ident.text(), *type, std::move(plist_result), std::move(body_result))
         : decls::function(ident.text(), std::move(plist_result), std::move(body_result));

      _scope.pop();
      return scope_insert(std::move(decl), ident);
	}

   rush::parse_result<ast::statement> parser::parse_function_body() {
      if (peek_skip_indent().is(symbols::arrow))
         return parse_function_expr_body();
      if (peek_skip_indent().is(symbols::colon))
         return parse_function_stmt_body();

      return nullptr; // todo: assert should never reach here.
   }

   rush::parse_result<ast::statement> parser::parse_function_expr_body() {
		assert(peek_skip_indent().is(symbols::arrow) && "expected an '=>' symbol.");
      next_skip_indent();

      auto stmt_result = rush::parse_result<ast::statement> {};

      auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         switch (tok.keyword()) {
         default: break;
         case keywords::pass_:
         case keywords::throw_:
            stmt_result = parse_stmt();
            if (stmt_result.failed()) return std::move(stmt_result);
            break;
         }
      }

      if (stmt_result.failed()) {
         auto expr = terminated(&parser::parse_expr);
         if (expr.failed()) return std::move(expr).as<ast::statement>();
         stmt_result = stmts::return_(std::move(expr));
      }

      std::vector<std::unique_ptr<ast::statement>> stmts;
      stmts.push_back(std::move(stmt_result));
      return stmts::block(std::move(stmts));
   }

   rush::parse_result<ast::statement> parser::parse_function_stmt_body() {
		assert(peek_skip_indent().is(symbols::colon) && "expected a ':' symbol.");
      next_skip_indent();

      if (peek_with_indent().is_not(symbols::indent)) {
         return error("expected start of function body '<indent>' before '{}'", next_with_indent());
      }

      return parse_block_stmt();
   }
}
