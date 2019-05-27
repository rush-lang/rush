#include "rush/ast/statements.hpp"

#include "rush/parser/parser.hpp"

namespace rush {
   std::unique_ptr<ast::statement> parser::parse_stmt() {
      auto tok = peek_skip_indent();
      if (tok.is_keyword()) {
         switch (tok.keyword()) {
            case keywords::let_: return ast::stmts::decl_stmt(parse_constant_decl());
            case keywords::var_: return ast::stmts::decl_stmt(parse_variable_decl());
            case keywords::if_: return parse_if_stmt();
            case keywords::for_: return parse_for_stmt();
            case keywords::while_: return parse_while_stmt();
            case keywords::return_: return parse_return_stmt();
         }
      }

      auto expr = parse_expr();
      return (expr != nullptr)
         ? ast::stmts::expr_stmt(std::move(expr))
         : nullptr;
   }

   std::unique_ptr<ast::statement_block> parser::parse_block_stmt() {
      assert(peek_with_indent().is(symbols::indent) && "expected indentation at start of block.");
      next_with_indent(); // consume indentation.

      std::vector<std::unique_ptr<ast::statement>> stmts;
      while (peek_with_indent().is_not(symbols::dedent)) {
         auto stmt = parse_stmt();
         if (!stmt) return nullptr;
         stmts.emplace_back(std::move(stmt));
      }

      next_with_indent(); // consume dedentation.
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
         : parse_stmt(); // stdc++ does not support move-only initializer-list ast::stmts::block({ parse_stmt() });
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
         return parse_compound_stmt();
      } else if (tok.is(symbols::colon)) {
         next_skip_indent(); // consume ':' symbol.

         _scope.push(scope_kind::block);
         auto else_ = (peek_with_indent().is(symbols::indent))
            ? parse_block_stmt()
            : parse_stmt(); // stdc++ does not support move-only initializer-list ast::stmts::block({ parse_stmt() });
         _scope.pop();

         return std::move(else_);
      }

      return error("expected a compound statement or block statement before '{}'", tok);
   }

   std::unique_ptr<ast::statement> parser::parse_for_stmt() {
      return nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_while_stmt() {
      return nullptr;
   }

   std::unique_ptr<ast::statement> parser::parse_return_stmt() {
      assert(peek_skip_indent().is(keywords::return_) && "expected 'return' keyword.");
      next_skip_indent(); // consume 'return' keyword.

      auto expr = parse_expr();
      if (!expr) return nullptr;
      return ast::stmts::return_(std::move(expr));
   }

   std::unique_ptr<ast::statement> parser::parse_compound_stmt() {
      auto tok = peek_skip_indent();
      switch (tok.keyword()) {
      default: return error("expected compound statement before '{}'", tok);
      case keywords::if_: return parse_if_stmt();
      }
   }
}
