#include "rush/parser/parser.hpp"

namespace decls = rush::ast::decls;

namespace rush {
   rush::parse_result<ast::module> parser::parse_module() {
      while (peek_skip_indent().is(keywords::import_)) {
         auto import_result = parse_import_decl();
         if (import_result.failed())
            return std::move(import_result).as<ast::module>();
         _module->push_back(std::move(import_result));
      }

      while (peek_skip_indent().is_not(symbols::eof)) {
         // ignore stray semi-colons
         if (peek_with_indent().is(symbols::semi_colon)) {
            next_with_indent();
            continue;
         }

         ast::module_access acc = ast::module_access::internal;
         if (peek_skip_indent().is(keywords::export_)) {
            next_skip_indent(); // consume 'export' keyword.
            acc = ast::module_access::exported;
         }

         auto decl_result = parse_toplevel_decl();
         if (decl_result.failed())
            return std::move(decl_result).as<ast::module>();
         _module->push_back(std::move(decl_result), acc);
      }

      return std::move(_module);
   }

   rush::parse_result<ast::import_declaration> parser::parse_import_decl() {
      assert(peek_skip_indent().is(keywords::import_) && "expected 'import' keyword");
      next_skip_indent(); // consume import keyword.

      // parse_qualified_name();
      auto tok = peek_skip_indent();
      if (!tok.is_identifier())
         return error("expected a name after import declaration and before '{}'", tok);

      std::string name = tok.text();
      next_skip_indent();

      while ((tok = peek_skip_indent()).is(symbols::period)) {
         next_skip_indent();
         tok = peek_skip_indent();
         if (!tok.is_identifier())
            return error("expected a qualified-name after import declaration and before '{}'", tok);

         name += ".";
         name += tok.text();
         next_skip_indent();
      }

      return decls::import(std::move(name));
   }

}
