
#include "rush/parser/parser.hpp"

namespace rush {
   ast::type_ref builtin_type_from_keyword(keywords::keyword_token_t kw) {
      switch (kw) {
         case keywords::void_: return ast::types::void_type;
         case keywords::bool_: return ast::types::bool_type;
         case keywords::byte_: return ast::types::uint8_type;
         case keywords::sbyte_: return ast::types::int8_type;
         case keywords::short_: return ast::types::int16_type;
         case keywords::ushort_: return ast::types::uint16_type;
         case keywords::int_: return ast::types::int32_type;
         case keywords::uint_: return ast::types::uint32_type;
         case keywords::long_: return ast::types::int64_type;
         case keywords::ulong_: return ast::types::uint64_type;
         case keywords::float_: return ast::types::ieee32_type;
         case keywords::double_: return ast::types::ieee64_type;
         case keywords::string_: return ast::types::string_type;
         case keywords::char_: return ast::types::char_type;
         default: return ast::types::error_type;
      }
   }

   std::optional<ast::type_ref> parser::parse_type() {
      auto tok = next_skip_indent();

      if (tok.is_keyword())
         return builtin_type_from_keyword(tok.keyword());

      if (tok.is_identifier()) {
         auto sym = _scope.current().lookup(tok.text());
         if (sym.is_undefined()) {
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
