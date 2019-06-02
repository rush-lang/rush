#pragma once

#ifndef RUSH_AST_EXPRS_TERNARY_HPP
#define RUSH_AST_EXPRS_TERNARY_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/types/utility.hpp"

#include <memory>

namespace rush::ast::exprs {
   std::unique_ptr<ternary_expression> ternary(
         std::unique_ptr<ast::expression> cond,
         std::unique_ptr<ast::expression> true_,
         std::unique_ptr<ast::expression> false_);
}

namespace rush::ast {
   class ternary_expression : public ast::expression {
      struct factory_tag_t {};

      friend std::unique_ptr<ternary_expression> exprs::ternary(
         std::unique_ptr<ast::expression> cond,
         std::unique_ptr<ast::expression> true_,
         std::unique_ptr<ast::expression> false_);

   public:
      ternary_expression(
         std::unique_ptr<ast::expression> cond,
         std::unique_ptr<ast::expression> true_,
         std::unique_ptr<ast::expression> false_,
         factory_tag_t)
         : _cond { std::move(cond) }
         , _true { std::move(true_) }
         , _false { std::move(false_) } {}

      ast::expression const& condition() const {
         return *_cond;
      }

      ast::expression const& true_expr() const {
         return *_true;
      }

      ast::expression const& false_expr() const {
         return *_false;
      }

      virtual ast::type_ref result_type() const override {
         return types::reduce(
            true_expr().result_type(),
            false_expr().result_type());
      };

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_ternary_expr(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {
         _cond->attach(*this, context);
         _true->attach(*this, context);
         _false->attach(*this, context);
      }

      virtual void detach(ast::node&, ast::context& context) override {
         _cond->detach(*this, context);
         _true->detach(*this, context);
         _false->detach(*this, context);
      }

   private:
      std::unique_ptr<ast::expression> _cond;
      std::unique_ptr<ast::expression> _true;
      std::unique_ptr<ast::expression> _false;
   };
}

namespace rush::ast::exprs {
   inline std::unique_ptr<ternary_expression> ternary(
         std::unique_ptr<ast::expression> cond,
         std::unique_ptr<ast::expression> true_,
         std::unique_ptr<ast::expression> false_) {
            return std::make_unique<ternary_expression>(
               std::move(cond),
               std::move(true_),
               std::move(false_),
               ternary_expression::factory_tag_t {});
         }
}

#endif // RUSH_AST_EXPRS_TERNARY_HPP
