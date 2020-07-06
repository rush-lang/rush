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
#include "rush/ast/statements.hpp"
#include "rush/diag/syntax_error.hpp"

namespace errs = rush::diag::errs;

namespace rush {

   rush::parse_result<ast::statement> parser::parse_stmt() {
      auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         auto kw = tok.keyword();

         auto simp = parser::parse_simple_stmt(kw);
         if (simp.second) {
            if (simp.first.failed())
               return std::move(simp.first);

            auto terminator_result = parse_terminator();
            return terminator_result != nullptr
               ? std::move(terminator_result)
               : std::move(simp.first);
         }

         auto comp = parse_compound_stmt(kw);
         if (comp.second) return std::move(comp.first);
      }

      auto expr_result = terminated(&parser::parse_expr);
      return (expr_result.success())
         ? ast::stmts::expr_stmt(std::move(expr_result))
         : std::move(expr_result).as<ast::statement>();
   }

   rush::parse_result<ast::statement> parser::parse_block_stmt() {
      assert(peek_with_indent().is(symbols::indent) && "expected indentation at start of block.");
      next_with_indent(); // consume indentation.

      auto results = std::vector<rush::parse_result<ast::statement>> {};
      auto tok = peek_with_indent();
      while (tok.is_not(symbols::dedent)) {
         // using a semi-colon cuts an expression short
         // and helps to dis-ambiguate between statements.
         if (tok.is(symbols::semi_colon)) {
            next_with_indent();
            continue;
         }

         auto result = parse_stmt();
         if (result.failed()) return std::move(result);
         results.push_back(std::move(result));
         tok = peek_with_indent();
      }

      next_with_indent(); // consume dedentation.
      auto stmts = std::vector<std::unique_ptr<ast::statement>> {};
      std::move(results.begin(), results.end(), std::back_inserter(stmts));
      return ast::stmts::block(std::move(stmts));
   }

   rush::parse_result<ast::statement> parser::parse_inline_stmt() {
      auto results = std::vector<rush::parse_result<ast::statement>> {};

      // using a semi-colon cuts an expression short
      // and helps to dis-ambiguate between statements.
      auto tok = peek_with_indent();
      if (tok.is_not(symbols::semi_colon)) {
         auto result = parse_stmt();
         if (result.failed()) return std::move(result);
         results.push_back(std::move(result));
      } else while (tok.is(symbols::semi_colon)) {
         next_with_indent();
         tok = peek_with_indent();
      }

      auto stmts = std::vector<std::unique_ptr<ast::statement>> {};
      std::move(results.begin(), results.end(), std::back_inserter(stmts));
      return ast::stmts::block(std::move(stmts));
   }

   rush::parse_result<ast::statement> parser::parse_if_stmt() {
      assert(peek_skip_indent().is(keywords::if_) && "expected 'if' keyword.");
      next_skip_indent(); // consume 'if' keyword.

      auto cond_result = parse_expr();
      if (cond_result.failed())
         return std::move(cond_result).as<ast::statement>();

      consume_skip_indent(symbols::colon); // consume optional ':'.
      auto then_result = (peek_with_indent().is(symbols::indent))
         ? parse_block_stmt()
         : peek_with_lbreak().is_not_any(symbols::lbreak, symbols::dedent)
         ? parse_inline_stmt()
         : ast::stmts::block();

      if (then_result.failed())
         return std::move(then_result);

      if (!peek_skip_indent().is(keywords::else_)) {
         return ast::stmts::if_(
            std::move(cond_result),
            std::move(then_result));
      }

      auto else_result = parse_else_stmt();
      if (else_result.failed())
         return std::move(else_result);

      return ast::stmts::if_(
         std::move(cond_result),
         std::move(then_result),
         std::move(else_result));
   }

   rush::parse_result<ast::statement> parser::parse_else_stmt() {
      assert(peek_skip_indent().is(keywords::else_) && "expected 'else' keyword");
      next_skip_indent(); // consume 'else' keyword

      consume_skip_indent(symbols::colon); // consume optional ':'.
      auto result = peek_with_indent().is(symbols::indent)
         ? parse_block_stmt()
         : peek_with_lbreak().is_not_any(symbols::lbreak, symbols::dedent, symbols::eof)
         ? parse_inline_stmt()
         : ast::stmts::block();
      return std::move(result);
   }

   rush::parse_result<ast::statement> parser::parse_for_stmt() {
      assert(peek_skip_indent().is(keywords::for_) && "expected 'for' keyword.");
      next_skip_indent(); // consume 'for' keyword.

      auto ptrn_result = peek_skip_indent().is_not(keywords::in_)
         ? parse_iteration_pattern()
         : ast::ptrns::discard();
      if (ptrn_result.failed())
         return std::move(ptrn_result).as<ast::statement>();

      if (!consume_skip_indent(keywords::in_))
         return errs::expected_for_in_expr(peek_skip_indent());

      auto expr_result = parse_expr();
      if (expr_result.failed())
         return std::move(expr_result).as<ast::statement>();

      consume_skip_indent(symbols::colon); // consume optional ':'.
      auto then_result = (peek_with_indent().is(symbols::indent))
         ? parse_block_stmt()
         : peek_with_lbreak().is_not_any(symbols::lbreak, symbols::dedent)
         ? parse_inline_stmt()
         : ast::stmts::block();
      if (then_result.failed())
         return std::move(then_result);

      return ast::stmts::for_(
         std::move(ptrn_result),
         std::move(expr_result),
         std::move(then_result));
   }

   rush::parse_result<ast::statement> parser::parse_while_stmt() {
      assert(peek_skip_indent().is(keywords::while_) && "expected 'while' keyword.");
      next_skip_indent(); // consume 'while' keyword.

      auto cond_result = parse_expr();
      if (cond_result.failed())
         return std::move(cond_result).as<ast::statement>();

      consume_skip_indent(symbols::colon); // consume optional ':'.
      auto then_result = (peek_with_indent().is(symbols::indent))
         ? parse_block_stmt()
         : peek_with_lbreak().is_not_any(symbols::lbreak, symbols::dedent)
         ? parse_inline_stmt()
         : ast::stmts::block();

      if (then_result.failed())
         return std::move(then_result);

      return ast::stmts::while_(
         std::move(cond_result),
         std::move(then_result));
   }

   rush::parse_result<ast::statement> parser::parse_return_stmt() {
      assert(peek_skip_indent().is(keywords::return_) && "expected 'return' keyword.");
      next_skip_indent(); // consume 'return' keyword.

      // return statements must only appear
      // at the end of a statement block in
      if (peek_skip_indent().is_any(
         symbols::semi_colon,
         symbols::dedent) ||
         peek_with_lbreak().is(symbols::lbreak))
         return ast::stmts::return_();

      auto result = parse_expr();
      return (result.success())
         ? ast::stmts::return_(std::move(result))
         : std::move(result).as<ast::statement>();
   }

   rush::parse_result<ast::statement> parser::parse_pass_stmt() {
      assert(peek_skip_indent().is(keywords::pass_) && "expected the 'pass' keyword.");
      next_skip_indent(); // consume 'pass' keyword.

      return ast::stmts::pass();
   }

   rush::parse_result<ast::statement> parser::parse_break_stmt() {
      assert(peek_skip_indent().is(keywords::break_) && "expected 'break' keyword.");
      next_skip_indent(); // consume 'break' keyword.

      return ast::stmts::break_();
   }

   rush::parse_result<ast::statement> parser::parse_continue_stmt() {
      assert(peek_skip_indent().is(keywords::continue_) && "expected 'continue' keyword.");
      next_skip_indent(); // consume 'continue' keyword.

      return ast::stmts::continue_();
   }

   rush::parse_result<ast::statement> parser::parse_switch_stmt() {
      assert(peek_skip_indent().is(keywords::switch_) && "expected 'switch' keyword.");
      next_skip_indent(); // consume 'switch' keyword.

      return nullptr;
   }

   rush::parse_result<ast::statement> parser::parse_throw_stmt() {
      assert(peek_skip_indent().is(keywords::throw_) && "expected 'throw' keyword.");
      next_skip_indent(); // consume 'throw' keyword.

      return nullptr;
   }

   rush::parse_result<ast::statement> parser::parse_try_stmt() {
      assert(peek_skip_indent().is(keywords::try_) && "expected 'try' keyword.");
      next_skip_indent(); // consume 'try' keyword.

      return nullptr;
   }

   rush::parse_result<ast::statement> parser::parse_yield_stmt() {
      assert(peek_skip_indent().is(keywords::yield_) && "expected 'yield' keyword.");
      next_skip_indent(); // consume 'yield' keyword.

      auto result = parse_expr();
      if (result.failed())
         return std::move(result).as<ast::statement>();
      return ast::stmts::yield_(std::move(result));
   }

   rush::parse_result<ast::statement> parser::parse_with_stmt() {
      assert(peek_skip_indent().is(keywords::with_) && "expected 'with' keyword.");
      next_skip_indent(); // consume 'with' keyword.

      return nullptr;
   }

   std::pair<rush::parse_result<ast::statement>, bool> parser::parse_simple_stmt(keyword_token_t kw) {
      rush::parse_result<ast::statement> result;

      switch (kw) {
      default: return { nullptr, false };
      case keywords::pass_: result = parse_pass_stmt(); break;
      case keywords::break_: result = parse_break_stmt(); break;
      case keywords::yield_: result = parse_yield_stmt(); break;
      case keywords::return_: result = parse_return_stmt(); break;
      case keywords::continue_: result = parse_continue_stmt(); break;
      case keywords::throw_: result = parse_throw_stmt(); break;
      case keywords::let_: {
         auto decl = parse_constant_decl();
         result = decl.success()
            ? ast::stmts::decl_stmt(std::move(decl))
            : std::move(decl).as<ast::statement>();
         break;
      }
      case keywords::var_: {
         auto decl = parse_variable_decl();
         result = decl.success()
            ? ast::stmts::decl_stmt(std::move(decl))
            : std::move(decl).as<ast::statement>();
         break;
      }}

      return { std::move(result), true };
   }

   std::pair<rush::parse_result<ast::statement>, bool> parser::parse_compound_stmt(keyword_token_t kw) {
      switch (kw) {
      default: return { nullptr, false };
      case keywords::if_: return { parse_if_stmt(), true };
      case keywords::for_: return { parse_for_stmt(), true };
      case keywords::while_: return { parse_while_stmt(), true };
      case keywords::switch_: return { parse_switch_stmt(), true };
      case keywords::try_: return { parse_try_stmt(), true };
      case keywords::with_: return { parse_with_stmt(), true };
      }
   }
}
