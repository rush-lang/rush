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
      auto tok = peek_skip_indent();
      auto result = rush::parse_type_result {};

      switch (tok.type()) {
      default: return errs::expected_type_annotation(tok); // todo: add proper error.
      case lexical_token_type::keyword:
      case lexical_token_type::identifier:
         result = parse_simple_type();
         break;
      case lexical_token_type::symbol:
         switch (tok.symbol()) {
         case symbols::left_parenthesis:
            result = parse_tuple_type();
            break;
         default: return errs::expected_type_annotation(tok);
         }
      }

      if (result.failed())
      return std::move(result);

      tok = peek_skip_indent();
      if (tok.is_symbol()) {
         switch (tok.symbol()) {
         case symbols::thin_arrow: return parse_function_type(result.type());
         case symbols::left_square_bracket: return parse_array_type(result.type());
         default: return std::move(result);
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

   rush::parse_type_result parser::parse_function_type(ast::type_ref params_type) {
      assert(peek_skip_indent().is(symbols::thin_arrow) && "expected thin arrow symbol '->'.");
      next_skip_indent();
      auto ret_type_result = parse_type();
      return _context->function_type(ret_type_result.type(), params_type);
   }

   rush::parse_type_result parser::parse_array_type(ast::type_ref elem_type) {
      assert(peek_skip_indent().is(symbols::left_square_bracket) && "expected array type symbol '['.");
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

   rush::parse_type_result parser::parse_tuple_type() {
      assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected start of tuple type '('.");
      next_skip_indent(); // consume '(' symbol.

      std::vector<ast::type_ref> types;
      do {
         auto result = parse_type();
         if (result.failed())
            return std::move(result);

         types.push_back(result.type());
         consume_skip_indent(symbols::comma);
      } while (peek_skip_indent().is_not(symbols::right_parenthesis));

      next_skip_indent();
      return types.size() == 1
         ? types.front()
         : _context->tuple_type(types);
   }
}
