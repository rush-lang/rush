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
#include "rush/ast/declarations.hpp"
#include "rush/ast/stmts/simple.hpp"
#include "rush/ast/stmts/result.hpp"

#include "rush/diag/syntax_error.hpp"

#include "rush/parser/parser.hpp"

namespace decls = rush::ast::decls;
namespace stmts = rush::ast::stmts;
namespace ptrns = rush::ast::ptrns;

namespace errs = rush::diag::errs;

namespace rush {

   rush::parse_result<ast::declaration> parser::parse_decl() {
      auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         switch (tok.keyword()) {
         case keywords::let_: return terminated(&parser::parse_constant_decl);
         case keywords::var_: return terminated(&parser::parse_variable_decl);
         case keywords::func_: return parse_function_decl();
         case keywords::class_: return parse_class_decl();
         case keywords::struct_: return parse_struct_decl();
         case keywords::import_: return parse_import_decl();
         case keywords::extern_: return parse_extern_decl();
         case keywords::static_: return parse_static_decl();
         case keywords::virtual_: return parse_virtual_decl();
         case keywords::abstract_: return parse_abstract_decl();
         case keywords::override_: return parse_override_decl();
         case keywords::async_: return parse_async_decl();
         case keywords::base_:
            if (peek_skip_indent(1).is(keywords::class_))
               return parse_base_decl();
            break;
         default: break;
         }
      }

      return {};
   }

   rush::parse_result<ast::declaration> parser::parse_toplevel_decl() {
      auto result = parse_decl();
      return result.success()
           ? std::move(result)
           : errs::expected_toplevel_decl(peek_skip_indent());
   }

   rush::parse_result<ast::declaration> parser::parse_extern_decl() {
      assert(peek_skip_indent(keywords::extern_) && "expected 'extern' keyword.");
      consume_skip_indent(keywords::extern_);

      auto tok = peek_skip_indent();
      auto result = tok.is(keywords::func_)
         ? terminated(&parser::parse_function_decl)
         : parse_decl();

      return result.success()
         ? decls::extern_(std::move(result))
         : std::move(result);
   }

   rush::parse_result<ast::declaration> parser::parse_async_decl() {
      assert(peek_skip_indent(keywords::async_) && "expected the 'async' keyword.");
      consume_skip_indent(keywords::async_);

      auto tok = peek_skip_indent();
      if (tok.is_not(keywords::func_))
         return errs::expected_function_decl(tok);

      auto result = parse_function_decl();
      return result.success()
           ? decls::async(std::move(result))
           : std::move(result);
   }

   rush::parse_result<ast::declaration> parser::parse_base_decl() {
      assert(peek_skip_indent(keywords::base_) && "expected the 'base' keyword.");
      consume_skip_indent(keywords::base_);

      auto tok = peek_skip_indent();
      if (tok.is_not(keywords::class_))
         return errs::expected_class_decl(tok);

      auto result = parse_class_decl();
      return result.success()
           ? decls::base(std::move(result))
           : std::move(result);
   }

   rush::parse_result<ast::declaration> parser::parse_static_decl() {
      assert(peek_skip_indent(keywords::static_) && "expected 'static' keyword.");
      consume_skip_indent(keywords::static_);

      auto result = parse_decl();
      return result.success()
           ? decls::static_(std::move(result))
           : std::move(result);
   }

   rush::parse_result<ast::declaration> parser::parse_virtual_decl() {
      assert(peek_skip_indent(keywords::virtual_) && "expected the 'virtual' keyword.");
      consume_skip_indent(keywords::virtual_);

      auto result = parse_decl();
      return result.success()
           ? decls::virtual_(std::move(result))
           : std::move(result);
   }

   rush::parse_result<ast::declaration> parser::parse_abstract_decl() {
      assert(peek_skip_indent(keywords::abstract_) && "expected the 'abstract' keyword.");
      consume_skip_indent(keywords::virtual_);

      auto result = parse_decl();
      return result.success()
           ? decls::abstract_(std::move(result))
           : std::move(result);
   }

   rush::parse_result<ast::declaration> parser::parse_override_decl() {
      assert(peek_skip_indent(keywords::override_) && "expected the 'override' keyword.");
      consume_skip_indent(keywords::override_);

      auto sealed = consume_skip_indent(symbols::exclamation_mark);
      auto result = parse_decl();
      return result.success()
           ? decls::override_(std::move(result))
           : std::move(result);
   }

	rush::parse_result<ast::declaration> parser::_parse_storage_decl(
		rush::function_ref<std::unique_ptr<ast::declaration>(std::unique_ptr<ast::pattern>)> fn) {
		auto ptrn = parse_storage_pattern();
      return ptrn.failed()
           ? std::move(ptrn).as<ast::declaration>()
           : rush::parse_result<ast::declaration> { fn(std::move(ptrn)) };
	}

	rush::parse_result<ast::declaration> parser::parse_constant_decl() {
		assert(peek_skip_indent(keywords::let_) && "expected the 'let' keyword.");
      consume_skip_indent(keywords::let_);
		return _parse_storage_decl([](auto ptrn) { return decls::let(std::move(ptrn)); });
	}

	rush::parse_result<ast::declaration> parser::parse_variable_decl() {
		assert(peek_skip_indent(keywords::var_) && "expected the 'var' keyword.");
      consume_skip_indent(keywords::var_);
		return _parse_storage_decl([](auto ptrn) { return decls::var(std::move(ptrn)); });
	}

	rush::parse_result<ast::pattern> parser::parse_parameter_list() {
		assert(peek_skip_indent().is(symbols::left_parenthesis) && "expected an opening parenthesis '('");
		next_skip_indent(); // consume '(' symbol.

      if (peek_skip_indent().is(symbols::right_parenthesis)) {
         next_skip_indent(); // consume ')' symbol.
         return ptrns::list(); // empty parameter list case.
      }

      auto params = parse_pattern_list([this]()
         -> rush::parse_result<ast::pattern> { return parse_parameter_pattern(); });
      if (params.failed()) return std::move(params);

      if (peek_skip_indent().is_not(symbols::right_parenthesis))
         return errs::expected(peek_skip_indent(), ")");
      next_skip_indent();

      return std::move(params);
	}


	rush::parse_result<ast::declaration> parser::parse_function_decl() {
		assert(peek_skip_indent().is_any(keywords::func_) && "expected the 'func' keyword.");
		next_skip_indent(); // consume func keyword.

		if (!peek_skip_indent().is_identifier())
         return errs::expected_function_name(peek_skip_indent());
		auto ident = next_skip_indent();

      auto plist_result = peek_skip_indent().is(symbols::left_parenthesis)
                        ? parse_parameter_list()
                        : ptrns::list();
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

      return std::move(decl);
	}

   rush::parse_result<ast::statement> parser::parse_function_body() {
      return peek_skip_indent().is(symbols::thick_arrow)
           ? terminated(&parser::parse_function_expr_body)
           : peek_skip_indent().is(symbols::colon_equals)
           ? terminated(&parser::parse_function_expr_body) // todo: change to parse math expression body.
           : parse_function_stmt_body();
   }

   rush::parse_result<ast::statement> parser::parse_function_expr_body() {
		assert(peek_skip_indent().is_any(symbols::thick_arrow, symbols::colon_equals) && "expected an '=>' or ':=' symbol.");
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

      return std::move(stmt_result);
   }

   rush::parse_result<ast::statement> parser::parse_function_stmt_body() {
      consume_skip_indent(symbols::colon); // consume optional ':'
      if (peek_with_indent().is_not(symbols::indent)) {
         return peek_with_lbreak().is_any(symbols::lbreak, symbols::dedent, symbols::semi_colon, symbols::eof)
              ? rush::parse_result<ast::statement> { stmts::block() }
              : errs::expected_function_stmt_body(peek_with_indent());
      }

      return parse_block_stmt();
   }

   rush::parse_result<ast::declaration> parser::parse_struct_decl() {
      assert(peek_skip_indent().is(keywords::struct_) && "expected the 'struct' keyword.");
      next_skip_indent(); // consume 'struct'

      if (!peek_skip_indent().is_identifier())
         return errs::expected_struct_name(peek_skip_indent());
		auto ident = next_skip_indent();

      consume_skip_indent(symbols::colon); // consume optional ':'
      std::vector<std::unique_ptr<ast::member_section_declaration>> sections;
      parse_result<ast::member_section_declaration> section_result;

      auto tok = peek_with_indent();
      if (tok.is(symbols::indent)) {
         section_result = parse_member_section(ast::member_access::public_);
         if (section_result.failed()) return std::move(section_result);
         sections.push_back(std::move(section_result));
      }

      while (peek_skip_indent().is_any(
         keywords::public_,
         keywords::private_,
         keywords::protected_)) {
            auto tok = next_skip_indent();
            consume_skip_indent(symbols::colon); // consume optional ':'

            // empty struct section.
            if (!peek_with_indent().is(symbols::indent))
               continue;

            switch (tok.keyword()) {
            default: break;
            case keywords::public_: section_result = parse_member_section(ast::member_access::public_); break;
            case keywords::private_: section_result = parse_member_section(ast::member_access::private_); break;
            case keywords::protected_: section_result = parse_member_section(ast::member_access::protected_); break;
            }

            if (section_result.failed()) return std::move(section_result);
            sections.push_back(std::move(section_result));
         }

      return decls::struct_(ident.text(), std::move(sections));
   }

   rush::parse_result<ast::declaration> parser::parse_class_decl() {
      assert(peek_skip_indent().is(keywords::class_) && "expected the 'class' keyword.");
      next_skip_indent(); // consume 'class'

		if (!peek_skip_indent().is_identifier())
         return errs::expected_class_name(peek_skip_indent());
		auto ident = next_skip_indent();

      consume_skip_indent(symbols::colon); // skip optional ':'
      std::vector<std::unique_ptr<ast::member_section_declaration>> sections;
      parse_result<ast::member_section_declaration> section_result;

      auto tok = peek_with_indent();
      if (tok.is(symbols::indent)) {
         section_result = parse_member_section(ast::member_access::private_);
         if (section_result.failed()) return std::move(section_result);
         sections.push_back(std::move(section_result));
      }

      while (peek_skip_indent().is_any(
         keywords::public_,
         keywords::private_,
         keywords::protected_)) {
            auto tok = next_skip_indent();
            consume_skip_indent(symbols::colon); // skip optional ':'

            // empty class section.
            if (!peek_with_indent().is(symbols::indent))
               continue;

            switch (tok.keyword()) {
            default: break;
            case keywords::public_: section_result = parse_member_section(ast::member_access::public_); break;
            case keywords::private_: section_result = parse_member_section(ast::member_access::private_); break;
            case keywords::protected_: section_result = parse_member_section(ast::member_access::protected_); break;
            }

            if (section_result.failed()) return std::move(section_result);
            sections.push_back(std::move(section_result));
         }

      return decls::class_(ident.text(), std::move(sections));
   }

   rush::parse_result<ast::member_section_declaration>
   parser::parse_member_section(ast::member_access acc) {
      assert(peek_with_indent().is(symbols::indent) && "expected indentation at start of block.");
      next_with_indent(); // consume indentation.

      auto results = std::vector<std::unique_ptr<ast::member_declaration>> {};
      while (peek_skip_indent().is_not(symbols::dedent)) {
         auto result = parse_member_decl();
         if (result.failed()) return std::move(result).as<ast::member_section_declaration>();
         results.push_back(std::move(result));
      }

      if (!peek_with_indent().is(symbols::dedent))
         return errs::expected(peek_skip_indent(), "<dedent>");
      next_with_indent(); // consume '<dedent>'

      return decls::member_section(acc, std::move(results));
   }

   rush::parse_result<ast::member_declaration> parser::parse_member_decl() {
      auto tok = peek_skip_indent();

      if (tok.is_keyword()) {
         rush::parse_result<ast::declaration> result;

         switch (tok.keyword()) {
         case keywords::let_: {
            result = terminated(&parser::parse_constant_decl);
            if (result.success()) return decls::field(
               std::move(result).as<ast::constant_declaration>());
            break;
         }
         case keywords::var_: {
            result = terminated(&parser::parse_variable_decl);
            if (result.success()) return decls::field(
               std::move(result).as<ast::variable_declaration>());
            break;
         }
         case keywords::get_: {
            auto r = parse_property_getter();
            if (r.success()) return std::move(r);
            result = std::move(r).as<ast::declaration>();
            break;
         }
         case keywords::set_: {
            auto r = parse_property_setter();
            if (r.success()) return std::move(r);
            result = std::move(r).as<ast::declaration>();
            break;
         }
         // case keywords::static_: {
         //    auto r = parse_static_member_decl();
         //    if (result.success()) return std::move(r);
         //    result = std::move(r).as<ast::declaration>();
         //    break;
         // }
         // case keywords::abstract_: {
         //    auto r = parse_abstract_member_decl();
         //    if (result.success()) return std::move(r);
         //    result = std::move(r).as<ast::declaration>();
         //    break;
         // }
         // case keywords::virtual_: {
         //    auto r = parse_virtual_member_decl();
         //    if (result.success()) return std::move(r);
         //    result = std::move(r).as<ast::declaration>();
         //    break;
         // }
         // case keywords::override_: {
         //    auto r = parse_override_member_decl();
         //    if (result.success()) return std::move(r);
         //    result = std::move(r).as<ast::declaration>();
         //    break;
         // }
         case keywords::func_: {
            result = parse_function_decl();
            if (result.success()) return decls::method(
               std::move(result).as<ast::function_declaration>());
            break;
         }
         case keywords::class_: {
            result = parse_class_decl();
            if (result.success()) return decls::nested(
               std::move(result).as<ast::type_declaration>());
            break;
         }
         case keywords::struct_: {
            result = parse_struct_decl();
            if (result.success()) return decls::nested(
               std::move(result).as<ast::type_declaration>());
            break;
         }
         default:
            result = errs::expected_member_decl(peek_skip_indent());
            break;
         }

         return std::move(result).as<ast::member_declaration>();
      }

      return errs::expected_member_decl(tok);
   }

   rush::parse_result<ast::member_declaration> parser::parse_property_getter() {
      assert(peek_skip_indent().is(keywords::get_) && "expected the 'get' keyword.");
      next_skip_indent(); // consume 'get'

      if (!peek_skip_indent().is_identifier())
         return errs::expected_property_name(peek_skip_indent());

      auto ident = next_skip_indent();

      auto type_result = rush::parse_type_result {};
      if (consume_skip_indent(symbols::thin_arrow)) {
         type_result = parse_type();
         if (type_result.failed())
            return std::move(type_result).errors();
      }

      if (peek_skip_indent().is_any(symbols::thick_arrow, symbols::colon_equals, symbols::colon)
       || peek_with_indent().is(symbols::indent)) {
         auto body_result = parse_function_body();
         if (body_result.failed())
            return std::move(body_result).as<ast::member_declaration>();

         return type_result.failed()
            ? decls::property_get(ident.text(), std::move(body_result))
            : decls::property_get(ident.text(), type_result.type(), std::move(body_result));
      }

      return type_result.failed()
         ? decls::auto_property_get(ident.text())
         : decls::auto_property_get(ident.text(), type_result.type());
   }

   rush::parse_result<ast::member_declaration> parser::parse_property_setter() {
      assert(peek_skip_indent().is(keywords::set_) && "expected the 'set' keyword.");
      next_skip_indent(); // consume 'set'

      if (!peek_skip_indent().is_identifier())
         return errs::expected_property_name(peek_skip_indent());

      auto ident = next_skip_indent();

      auto type_result = rush::parse_type_result {};
      if (consume_skip_indent(symbols::thin_arrow)) {
         type_result = parse_type();
         if (type_result.failed())
            return std::move(type_result).errors();
      }

      if (peek_skip_indent().is_any(symbols::thick_arrow, symbols::colon_equals, symbols::colon)
       || peek_with_indent().is(symbols::indent)) {
         auto body_result = parse_function_body();
         if (body_result.failed())
            return std::move(body_result).as<ast::member_declaration>();

         return type_result.failed()
            ? decls::property_set(ident.text(), std::move(body_result))
            : decls::property_set(ident.text(), type_result.type(), std::move(body_result));
      }

      return type_result.failed()
         ? decls::auto_property_set(ident.text())
         : decls::auto_property_set(ident.text(), type_result.type());
   }
}
