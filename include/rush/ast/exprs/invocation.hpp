#pragma once

#ifndef RUSH_AST_EXPRS_INVOCATION_HPP
#define RUSH_AST_EXPRS_INVOCATION_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/list.hpp"

#include <string>
#include <memory>

namespace rush::ast {
   class invocation_expression;
}

namespace rush::ast::exprs {
   std::unique_ptr<invocation_expression> invocation(
      std::unique_ptr<ast::expression> callable,
      std::unique_ptr<ast::argument_list> args);
}

namespace rush::ast {
	class invocation_expression : public expression {
      struct factory_tag_t {};

      friend std::unique_ptr<invocation_expression> exprs::invocation(
         std::unique_ptr<ast::expression> callable,
         std::unique_ptr<ast::argument_list> args);

   public:
      invocation_expression(
         std::unique_ptr<ast::expression> callable,
         std::unique_ptr<ast::argument_list> args,
         factory_tag_t)
         : _callable { std::move(callable) }
         , _args { std::move(args) } {}

      ast::expression const& callable() const noexcept {
         return *_callable;
      }

      ast::argument_list const& arguments() const noexcept {
         return *_args;
      }

      virtual ast::type_ref result_type() const override;

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_invocation_expr(*this);
      }

   private:
      std::unique_ptr<ast::expression> _callable;
      std::unique_ptr<ast::argument_list> _args;
   };

	namespace exprs {
		std::unique_ptr<invocation_expression> call(std::string name);
	}
}

namespace rush::ast::exprs {
   inline std::unique_ptr<invocation_expression> invocation(
      std::unique_ptr<ast::expression> callable,
      std::unique_ptr<ast::argument_list> args) {
         return std::make_unique<invocation_expression>(
            std::move(callable),
            std::move(args),
            invocation_expression::factory_tag_t {});
      }
}

#endif // RUSH_AST_EXPRS_INVOCATION_HPP