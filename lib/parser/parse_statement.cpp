#include "rush/ast/statements.hpp"

#include "rush/parser/parser.hpp"

namespace rush {
   std::unique_ptr<ast::statement> parser::parse_stmt() {
      auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         auto kw = tok.keyword();

         auto simp = parse_simple_stmt(kw);
         if (simp.second) return std::move(simp.first);

         auto comp = parse_compound_stmt(kw);
         if (comp.second) return std::move(comp.first);

         switch (kw) {
            default: return error("'{}' is not a valid statement.", tok);
            // these keywords may form part of a expression statement
            case keywords::base_:
            case keywords::this_: break;
            // case keywords::pass_: return nullptr;
         }
      }

      auto expr = parse_expr();
      return (expr != nullptr)
         ? ast::stmts::expr_stmt(std::move(expr))
         : nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_block_stmt() {
      assert(peek_with_indent().is(symbols::indent) && "expected indentation at start of block.");
      next_with_indent(); // consume indentation.

      std::vector<std::unique_ptr<ast::statement>> stmts;
      while (peek_with_indent().is_not(symbols::dedent)) {
         auto stmt = parse_terminated_stmt();
         if (!stmt) return nullptr;
         stmts.emplace_back(std::move(stmt));
      }

      next_with_indent(); // consume dedentation.
      return ast::stmts::block(std::move(stmts));
   }

   std::unique_ptr<ast::statement> parser::parse_inline_stmt() {
      auto stmt = parse_terminated_stmt();
      if (!stmt) return nullptr;

      std::vector<std::unique_ptr<ast::statement>> stmts;
      stmts.emplace_back(std::move(stmt));
      return ast::stmts::block(std::move(stmts));
   }

   std::unique_ptr<ast::statement> parser::parse_if_stmt() {
      assert(peek_skip_indent().is(keywords::if_) && "expected 'if' keyword.");
      next_skip_indent(); // consume 'if' keyword.

      auto cond = parse_expr();
      if (!cond) return nullptr;

      if (!peek_skip_indent().is(symbols::colon))
         return error("expected body of 'if' statement before '{}'", next_skip_indent());
      next_skip_indent(); // consume ':' symbol.

      _scope.push(scope_kind::block);
      auto then = (peek_with_indent().is(symbols::indent))
         ? parse_block_stmt()
         : parse_inline_stmt();
      _scope.pop();

      if (!peek_skip_indent().is(keywords::else_)) {
         return ast::stmts::if_(
            std::move(cond),
            std::move(then));
      }

      auto else_ = parse_else_stmt();
      if (!else_) return nullptr;
      return ast::stmts::if_(
         std::move(cond),
         std::move(then),
         std::move(else_));
   }

   std::unique_ptr<ast::statement> parser::parse_else_stmt() {
      assert(peek_skip_indent().is(keywords::else_) && "expected 'else' keyword");
      next_skip_indent(); // consume 'else' keyword

      auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         auto stmt = parse_compound_stmt(tok.keyword());
         if (stmt.second) return std::move(stmt.first);
         return error("expected compound statement before '{}'", tok);
      } else if (tok.is(symbols::colon)) {
         next_skip_indent(); // consume ':' symbol.

         _scope.push(scope_kind::block);
         auto else_ = (peek_with_indent().is(symbols::indent))
            ? parse_block_stmt()
            : parse_inline_stmt();
         _scope.pop();

         return std::move(else_);
      }

      return error("expected a compound statement or block statement before '{}'", tok);
   }

   std::unique_ptr<ast::statement> parser::parse_for_stmt() {
      assert(peek_skip_indent().is(keywords::for_) && "expected 'for' keyword.");
      next_skip_indent(); // consume 'for' keyword.

      return nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_while_stmt() {
      assert(peek_skip_indent().is(keywords::while_) && "expected 'while' keyword.");
      next_skip_indent(); // consume 'while' keyword.

      auto cond = parse_expr();
      if (!cond) return nullptr;

      if (!peek_skip_indent().is(symbols::colon))
         return error("expected body of 'while' statement before '{}'", next_skip_indent());
      next_skip_indent(); // consume ':' symbol.

      _scope.push(scope_kind::block);
      auto then = (peek_with_indent().is(symbols::indent))
         ? parse_block_stmt()
         : parse_inline_stmt();
      _scope.pop();

      return ast::stmts::while_(
         std::move(cond),
         std::move(then));
   }

   std::unique_ptr<ast::statement> parser::parse_return_stmt() {
      assert(peek_skip_indent().is(keywords::return_) && "expected 'return' keyword.");
      next_skip_indent(); // consume 'return' keyword.

      if (peek_skip_indent().is(symbols::semi_colon))
         return ast::stmts::return_();

      auto expr = parse_expr();
      return (expr)
         ? ast::stmts::return_(std::move(expr))
         : nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_terminated_stmt() {
      // using a semi-colon cuts an expression short
      // and helps to dis-ambiguate between statements.
      auto tok = peek_skip_indent();
      while (tok.is(symbols::semi_colon)) {
         next_skip_indent();
         tok = peek_skip_indent();
      }

      return terminated(&parser::parse_stmt);
   }

   std::unique_ptr<ast::statement> parser::parse_pass_stmt() {
      assert(peek_skip_indent().is(keywords::pass_) && "expected the 'pass' keyword.");
      next_skip_indent(); // consume 'pass' keyword.

      return ast::stmts::pass();
   }

   std::unique_ptr<ast::statement> parser::parse_break_stmt() {
      assert(peek_skip_indent().is(keywords::break_) && "expected 'break' keyword.");
      next_skip_indent(); // consume 'break' keyword.

      return ast::stmts::break_();
   }

   std::unique_ptr<ast::statement> parser::parse_continue_stmt() {
      assert(peek_skip_indent().is(keywords::continue_) && "expected 'continue' keyword.");
      next_skip_indent(); // consume 'continue' keyword.

      return ast::stmts::continue_();
   }

   std::unique_ptr<ast::statement> parser::parse_switch_stmt() {
      assert(peek_skip_indent().is(keywords::switch_) && "expected 'switch' keyword.");
      next_skip_indent(); // consume 'switch' keyword.

      return nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_throw_stmt() {
      assert(peek_skip_indent().is(keywords::throw_) && "expected 'throw' keyword.");
      next_skip_indent(); // consume 'throw' keyword.

      return nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_try_stmt() {
      assert(peek_skip_indent().is(keywords::try_) && "expected 'try' keyword.");
      next_skip_indent(); // consume 'try' keyword.

      return nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_yield_stmt() {
      assert(peek_skip_indent().is(keywords::yield_) && "expected 'yield' keyword.");
      next_skip_indent(); // consume 'yield' keyword.

      auto expr = parse_expr();
      if (!expr) return nullptr;
      return ast::stmts::yield_(std::move(expr));
   }

   std::unique_ptr<ast::statement> parser::parse_with_stmt() {
      assert(peek_skip_indent().is(keywords::with_) && "expected 'with' keyword.");
      next_skip_indent(); // consume 'with' keyword.

      return nullptr;
   }

   std::pair<std::unique_ptr<ast::statement>, bool> parser::parse_simple_stmt(keyword_token_t kw) {
      std::unique_ptr<ast::statement> stmt;

      switch (kw) {
      default: return { nullptr, false };
      case keywords::pass_: stmt = parse_pass_stmt(); break;
      case keywords::break_: stmt = parse_break_stmt(); break;
      case keywords::yield_: stmt = parse_yield_stmt(); break;
      case keywords::return_: stmt = parse_return_stmt(); break;
      case keywords::continue_: stmt = parse_continue_stmt(); break;
      case keywords::throw_: stmt = parse_throw_stmt(); break;
      case keywords::let_: {
         auto decl = parse_constant_decl();
         if (decl) stmt = ast::stmts::decl_stmt(std::move(decl));
         break;
      }
      case keywords::var_: {
         auto decl = parse_variable_decl();
         if (decl) stmt = ast::stmts::decl_stmt(std::move(decl));
         break;
      }}

      return { std::move(stmt), true };
   }

   std::pair<std::unique_ptr<ast::statement>, bool> parser::parse_compound_stmt(keyword_token_t kw) {
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
