#pragma once

#ifndef RUSH_AST_EXPRS_MEMBER_HPP
#define RUSH_AST_EXPRS_MEMBER_HPP

#include "rush/ast/exprs/expression.hpp"

namespace rush::ast {
   class member_access_expression : public ast::expression {
   public:
      member_access_expression(
         std::unique_ptr<ast::expression> expr,
         std::unique_ptr<ast::expression> member)
         : _expr { std::move(expr) }
         , _member { std::move(member) } {}

      bool is_unresolved() const noexcept {
         return true;
      }

      ast::expression const& expression() const noexcept {
         return *_expr;
      }

      ast::expression const& member() const noexcept {
         return *_member;
      }

      virtual ast::type_ref result_type() const override {
         return member().result_type();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         return v.visit_member_access_expr(*this);
      }

      virtual void attach(ast::node&, ast::context& ctx) override {
         _expr->attach(*this, ctx);
         _member->attach(*this, ctx);
      }

      virtual void detach(ast::node&, ast::context& ctx) override {
         _expr->detach(*this, ctx);
         _member->detach(*this, ctx);
      }

   private:
      std::unique_ptr<ast::expression> _expr;
      std::unique_ptr<ast::expression> _member;
   };

   namespace exprs {
      inline std::unique_ptr<member_access_expression> member_access(
         std::unique_ptr<expression> expr,
         std::unique_ptr<expression> member) {
            return std::make_unique<member_access_expression>(
               std::move(expr),
               std::move(member));
         }
   }
}

#endif // RUSH_AST_EXPRS_MEMBER_HPP
