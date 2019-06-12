#include "rush/parser/parser.hpp"

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

   std::optional<ast::type_ref> parser::parse_type() {
      auto tok = next_skip_indent();

      if (tok.is_keyword())
         return builtin_type_from_keyword(tok.keyword(), *_context);

      if (tok.is_identifier()) {
         auto sym = _scope.current().lookup(tok.text());
         if (sym.is_undefined()) {
            // todo: push error up into parse result.
            // ast::type_ref not being a derivative of ast::node makes this difficult.
            error("the type or namespace '{}' could not be found", tok);
            return std::nullopt;
         }

         auto& decl = *sym.declaration();
         switch (decl.kind()) {
            case ast::declaration_kind::alias:
            case ast::declaration_kind::enum_:
            case ast::declaration_kind::class_:
            case ast::declaration_kind::struct_:
            case ast::declaration_kind::concept:
            case ast::declaration_kind::interface: return decl.type();
            case ast::declaration_kind::constant:
               error("'{}' is a constant but is used like a type.", tok);
               return std::nullopt;
            case ast::declaration_kind::variable:
               error("'{}' is a variable but is used like a type.", tok);
               return std::nullopt;
            case ast::declaration_kind::function:
               error("'{}' is a function but is used like a type.", tok);
               return std::nullopt;
            case ast::declaration_kind::extension:
            default:
               error("parse_type unexpected!", tok);
               return std::nullopt;
         }
      }; // todo: implement.

      return std::nullopt;
   }

   std::optional<ast::type_ref> parser::parse_type_annotation() {
      assert(peek_skip_indent().is(symbols::colon) && "expected a type annotation symbol ':'");
      next_skip_indent();

      return parse_type();
   }
}
