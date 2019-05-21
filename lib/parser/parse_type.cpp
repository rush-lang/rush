
#include "rush/parser/parser.hpp"

namespace rush {
   ast::type_ref builtin_type_from_keyword(keywords::keyword_token_t kw) {
      switch (kw) {
         case keywords::void_: return ast::types::void_type;
         case keywords::bool_: return ast::types::bool_type;
         case keywords::byte_: return ast::types::byte_type;
         case keywords::sbyte_: return ast::types::sbyte_type;
         case keywords::short_: return ast::types::short_type;
         case keywords::ushort_: return ast::types::ushort_type;
         case keywords::int_: return ast::types::int_type;
         case keywords::uint_: return ast::types::uint_type;
         case keywords::long_: return ast::types::long_type;
         case keywords::ulong_: return ast::types::ulong_type;
         case keywords::float_: return ast::types::float_type;
         case keywords::double_: return ast::types::double_type;
         case keywords::string_: return ast::types::string_type;
         case keywords::char_: return ast::types::char_type;
         default: return ast::types::error_type;
      }
   }

   std::optional<ast::type_ref> parser::parse_type_annotation() {
      assert(peek_skip_indent().is(symbols::colon) && "expected a type annotation symbol ':'");
      next_skip_indent();

      auto tok = next_skip_indent();

      if (tok.is_keyword()) return builtin_type_from_keyword(tok.keyword());
      if (tok.is_identifier()) return ast::types::error_type; // todo: implement.

      error("expected a type identifier before '{}'", tok);
      return std::nullopt;
   }
}
