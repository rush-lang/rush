#include "rush/ast/declarations.hpp"
#include "rush/ast/stmts/simple.hpp"
#include "rush/ast/stmts/result.hpp"

#include "rush/diag/syntax_error.hpp"

#include "rush/parser/parser.hpp"

namespace decls = rush::ast::decls;
namespace stmts = rush::ast::stmts;

namespace errs = rush::diag::errs;

namespace rush {

   rush::parse_result<ast::declaration> parser::scope_insert(std::unique_ptr<ast::declaration> decl, rush::lexical_token const& ident) {
      return (!_scope.insert({ *decl }))
         ? errs::definition_already_exists(ident)
         : rush::parse_result<ast::declaration> { std::move(decl) };
   }

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

      return errs::expected_toplevel_decl(tok);
   }

	rush::parse_result<ast::declaration> parser::_parse_storage_decl(std::string storage_type,
		rush::function_ref<std::unique_ptr<ast::declaration>(std::string, ast::type_ref, std::unique_ptr<ast::expression>)> fn
	) {
		if (peek_skip_indent().is(symbols::left_bracket)) {
			// parse_destructure_pattern.
         return errs::not_supported(peek_skip_indent(), "destructuring");
		}

      auto ident = peek_skip_indent();
		if (!ident.is_identifier())
         return errs::expected_identifier(peek_skip_indent());
      next_skip_indent();

      auto type_result = rush::parse_type_result {};
      if (peek_skip_indent().is(symbols::colon)) {
         type_result = parse_type_annotation();
         if (type_result.failed())
            return std::move(type_result).errors();
         if (type_result.is_undefined())
            return errs::expected_type_annotation(peek_skip_indent());
      }

      auto init = rush::parse_result<ast::expression> {};
      if (peek_skip_indent().is(symbols::equals)) {
         next_skip_indent();
         if ((init = parse_expr()).failed())
            return std::move(init).as<ast::declaration>();
      } else if (type_result.failed()) {
         return std::move(type_result).errors();
      } else if (type_result.is_undefined()) {
         if (storage_type == "constant") return errs::constant_requires_type_annotation(ident);
         if (storage_type == "variable") return errs::variable_requires_type_annotation(ident);
      }

      type_result = type_result.success() ? std::move(type_result) : ast::types::undefined;
      auto decl = fn(ident.text(), type_result.type(), std::move(init));
      if (!decl) return std::move(decl); // todo: should throw here an error here. only reason this should fail is bad-mem-alloc.

      // return scope_insert(std::move(decl), ident);
      if (!_scope.insert({ *decl })) {
         if (storage_type == "constant") return errs::local_constant_name_previously_defined(ident);
         if (storage_type == "variable") return errs::local_variable_name_previously_defined(ident);
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
            return errs::expected_parameter_name(peek_skip_indent());

         // if (peek_skip_indent().is_not(symbols::colon))
         //    return errs::expected_type_annotation(peek_skip_indent());

         // auto type_result = parse_type_annotation();
         // if (type_result.failed()) return std::move(type_result).errors();

         // std::transform(
         //    std::make_move_iterator(names.begin()),
         //    std::make_move_iterator(names.end()),
         //    std::back_inserter(results), [this, &type_result](auto n) {
         //       auto p = decls::param(n.text(), type_result.type());
         //       return !_scope.insert({ *p })
         //          ? errs::parameter_redefinition(n)
         //          : rush::parse_result<ast::parameter> { std::move(p) };
         // });

         auto type = ast::types::undefined;
         if (peek_skip_indent().is(symbols::colon)) {
            auto type_result = parse_type_annotation();
            if (type_result.failed()) return std::move(type_result).errors();
            type = type_result.type();
         }

         std::transform(
            std::make_move_iterator(names.begin()),
            std::make_move_iterator(names.end()),
            std::back_inserter(results), [this, &type](auto n) {
               auto p = decls::param(n.text(), type);
               return !_scope.insert({ *p })
                  ? errs::parameter_redefinition(n)
                  : rush::parse_result<ast::parameter> { std::move(p) };
         });

      } while (consume_skip_indent(symbols::comma));

      // check for any parameter parsing failures.
      if (std::any_of(results.begin(), results.end(), [](auto& p) { return p.failed(); }))
         return nullptr; // todo: accumulate parameter errors and return.

      if (peek_skip_indent().is_not(symbols::right_parenthesis))
         return errs::expected(peek_skip_indent(), ")");
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
         return errs::expected_function_name(peek_skip_indent());
		auto ident = next_skip_indent();

		if (peek_skip_indent().is_not(symbols::left_parenthesis))
         return errs::expected(peek_skip_indent(), "(");

		auto plist_result = parse_parameter_list();
		if (plist_result.failed())
         return std::move(plist_result).as<ast::declaration>();

      auto type_result = rush::parse_type_result {};
      if (peek_skip_indent().is(symbols::thin_arrow)) {
         next_skip_indent();
         type_result = parse_type();
         if (type_result.failed())
            return std::move(type_result).errors();
      }

		auto body_result = parse_function_body();
      if (body_result.failed())
         return std::move(body_result).as<ast::declaration>();

		auto decl = type_result.success()
         ? decls::function(ident.text(), type_result.type(), std::move(plist_result), std::move(body_result))
         : decls::function(ident.text(), std::move(plist_result), std::move(body_result));

      _scope.pop();
      return scope_insert(std::move(decl), ident);
	}

   rush::parse_result<ast::statement> parser::parse_function_body() {
      if (peek_skip_indent().is(symbols::thick_arrow))
         return terminated(&parser::parse_function_expr_body);
      if (peek_skip_indent().is(symbols::colon))
         return parse_function_stmt_body();

      return nullptr; // todo: assert should never reach here.
   }

   rush::parse_result<ast::statement> parser::parse_function_expr_body() {
		assert(peek_skip_indent().is(symbols::thick_arrow) && "expected an '=>' symbol.");
      next_skip_indent();

      auto stmt_result = rush::parse_result<ast::statement> {};

      auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         auto kw = tok.keyword();
         switch (kw) {
         default: break;
         case keywords::pass_:
         case keywords::throw_:
            stmt_result = parse_simple_stmt(kw).first;
            if (stmt_result.failed()) return std::move(stmt_result);
            break;
         }
      }

      if (stmt_result.failed()) {
         auto expr = parser::parse_expr();
         if (expr.failed()) return std::move(expr).as<ast::statement>();
         stmt_result = stmts::return_(std::move(expr));
      }

      std::vector<std::unique_ptr<ast::statement>> stmts;
      stmts.push_back(std::move(stmt_result));
      return stmts::block(std::move(stmts));
   }

   rush::parse_result<ast::statement> parser::parse_function_stmt_body() {
		assert(peek_skip_indent().is(symbols::colon) && "expected a ':' symbol.");
      next_skip_indent(); // consume ':'

      if (peek_with_indent().is_not(symbols::indent))
         return errs::expected_function_stmt_body(peek_with_indent());

      return parse_block_stmt();
   }
}
