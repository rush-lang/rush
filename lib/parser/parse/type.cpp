#include "rush/parser/parser.hpp"
#include "rush/diag/syntax_error.hpp"

namespace errs = rush::diag::errs;

namespace rush {
   ast::type_ref builtin_type_from_keyword(keywords::keyword_token_t kw, ast::context& ctx) {
      switch (kw) {
         case keywords::void_: return ast::types::void_type;
         case keywords::bool_: return ctx.bool_type();
         case keywords::byte_: return ctx.int8_type();
         case keywords::sbyte_: return ctx.int8_type();
         case keywords::short_: return ctx.int16_type();
         case keywords::ushort_: return ctx.uint16_type();
         case keywords::int_: return ctx.int32_type();
         case keywords::uint_: return ctx.uint32_type();
         case keywords::long_: return ctx.int64_type();
         case keywords::ulong_: return ctx.uint64_type();
         case keywords::float_: return ctx.ieee32_type();
         case keywords::double_: return ctx.ieee64_type();
         case keywords::string_: return ctx.string_type();
         case keywords::char_: return ctx.char_type();
         default: return ast::types::undefined;
      }
   }

    rush::parse_type_result parser::parse_type() {
      auto result = parse_simple_type();
      if (result.failed()) return result;

      auto tok = peek_skip_indent();
      if (tok.is_symbol()) {
         switch (tok.symbol()) {
         default: break; // not a symbol that extends a result.
         case symbols::left_square_bracket: return parse_array_type(result.type());
         }
      }

      return std::move(result);
   }

   rush::parse_type_result parser::parse_simple_type() {
      auto tok = next_skip_indent();

      if (tok.is_keyword())
         return builtin_type_from_keyword(tok.keyword(), *_context);

      if (tok.is_identifier()) {
         auto sym = _scope.current().lookup(tok.text());
         if (sym.is_undefined()) return ast::types::undefined;

         auto& decl = *sym.declaration();
         switch (decl.kind()) {
            case ast::declaration_kind::alias:
            case ast::declaration_kind::enum_:
            case ast::declaration_kind::class_:
            case ast::declaration_kind::struct_:
            case ast::declaration_kind::concept:
            case ast::declaration_kind::interface: return decl.type();
            case ast::declaration_kind::constant: return errs::constant_used_like_type(tok);
            case ast::declaration_kind::variable: return errs::variable_used_like_type(tok);
            case ast::declaration_kind::function: return errs::function_used_like_type(tok);
            default: return errs::not_supported(tok, "parsed type");
         }
      }; // todo: implement.

      return ast::types::undefined;
   }

   rush::parse_type_result parser::parse_type_annotation() {
      assert(peek_skip_indent().is(symbols::colon) && "expected a type annotation symbol ':'");
      next_skip_indent();

      return parse_type();
   }

   rush::parse_type_result parser::parse_array_type(ast::type_ref elem_type) {
      assert(peek_skip_indent().is(symbols::left_square_bracket) && "expected array type");
      next_skip_indent();

      auto tok = peek_skip_indent();
      while (tok.is(symbols::comma)) {
         next_skip_indent();
         // grab dimensions etc..
         tok = peek_skip_indent();
      }

      if (tok.is_not(symbols::right_square_bracket))
         return errs::expected_closing_square_bracket(tok);
      next_skip_indent(); // skip closing ']'
      return _context->array_type(elem_type);
   }
}
