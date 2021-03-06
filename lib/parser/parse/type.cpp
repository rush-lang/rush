/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#include "rush/parser/parser.hpp"
#include "rush/diag/syntax_error.hpp"

namespace errs = rush::diag::errs;

namespace rush {
   ast::type_ref builtin_type_from_keyword(keywords::keyword_token_t kw, ast::context& ctx) {
      switch (kw) {
         case keywords::void_: return ast::types::void_type;
         case keywords::bool_: return ctx.bool_type();
         case keywords::byte_: return ctx.uint8_type();
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
      bool istuple = false;

      switch (tok.type()) {
      default: return errs::expected_type_annotation(tok); // todo: add proper error.
      case lexical_token_type::keyword:
      case lexical_token_type::identifier:
         result = parse_simple_type();
         break;
      case lexical_token_type::symbol:
         switch (tok.symbol()) {
         case symbols::ampersat:
            next_skip_indent();
            result = parse_type();
            if (result.failed()) return std::move(result);
            return _context->type_extension(result.type());
         case symbols::left_parenthesis:
            result = parse_tuple_type();
            istuple = true;
            break;
         case symbols::left_bracket:
            result = parse_array_type();
            break;
         default: return errs::expected_type_annotation(tok);
         }
      }

      if (result.failed())
         return std::move(result);

      tok = peek_skip_indent();
      if (tok.is_symbol()) {
         switch (tok.symbol()) {
         case symbols::question_mark:
            next_skip_indent();
            return _context->optional_type(result.type());
         case symbols::thin_arrow:
            return parse_function_type(!istuple
                 ? _context->tuple_type(result.type())
                 : result.type());
         // case symbols::left_bracket: return parse_array_type(result.type());
         default: return std::move(result);
         }
      }

      return std::move(result);
   }

   rush::parse_type_result parser::parse_simple_type() {
      auto tok = next_skip_indent();
      return tok.is_keyword()
           ? builtin_type_from_keyword(tok.keyword(), *_context)
           : tok.is_identifier()
           ? _context->named_type(tok.text())
           : ast::types::undefined;
   }

   rush::parse_type_result parser::parse_type_annotation() {
      assert(peek_skip_indent().is(symbols::colon) && "expected a type annotation symbol ':'");
      next_skip_indent();

      return parse_type();
   }

   rush::parse_type_result parser::parse_function_type(ast::type_ref params_type) {
      assert(peek_skip_indent().is(symbols::thin_arrow) && "expected thin arrow symbol '->'.");
      next_skip_indent(); // consume '->'
      auto ret_type_result = parse_type();
      return _context->function_type(ret_type_result.type(), params_type);
   }

   rush::parse_type_result parser::parse_array_type() {
      assert(peek_skip_indent().is(symbols::left_bracket) && "expected array type symbol '['.");
      next_skip_indent(); // consume '['

      auto tok = peek_skip_indent();
      if (tok.is_integer_literal()) {
         // consume fixed size.
         next_skip_indent(); // consume integer literal.
         next_skip_indent(); // consume ':'
      }

      // while (tok.is(symbols::comma)) {
      //    next_skip_indent();
      //    // grab dimensions etc..
      //    tok = peek_skip_indent();
      // }

      auto elem_type_result = parse_type();
      if (elem_type_result.failed())
         return std::move(elem_type_result);


      tok = peek_skip_indent();
      if (tok.is_not(symbols::right_bracket))
         return errs::expected_closing_square_bracket(tok);
      next_skip_indent(); // skip closing ']'

      return _context->array_type(elem_type_result.type());
   }

   rush::parse_type_result parser::parse_tuple_type() {
      assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected start of tuple type '('.");
      next_skip_indent(); // consume '(' symbol.

      std::vector<ast::type_ref> types;
      while (!consume_skip_indent(symbols::right_parenthesis)) {
         auto result = parse_type();
         if (result.failed())
            return std::move(result);

         types.push_back(result.type());
         consume_skip_indent(symbols::comma);
      }

      return types.size() == 1
         ? types.front()
         : _context->tuple_type(types);
   }
}
