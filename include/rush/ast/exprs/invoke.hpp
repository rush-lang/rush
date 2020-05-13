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
#pragma once

#ifndef RUSH_AST_EXPRS_INVOCATION_HPP
#define RUSH_AST_EXPRS_INVOCATION_HPP

#include "rush/ast/types/builtin.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/exprs/argument.hpp"

#include <string>
#include <memory>

namespace rush::ast {
   class invoke_expression;
}

namespace rush::ast::exprs {
   std::unique_ptr<invoke_expression> invoke(
      std::unique_ptr<ast::expression> callable,
      std::unique_ptr<ast::argument_list> args);
}

namespace rush::ast {
	class invoke_expression : public expression {
      struct factory_tag_t {};

      friend std::unique_ptr<invoke_expression> exprs::invoke(
         std::unique_ptr<ast::expression> callable,
         std::unique_ptr<ast::argument_list> args);

   public:
      invoke_expression(
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
         v.visit_invoke_expr(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {
         _callable->attach(*this, context);
         _args->attach(*this, context);
      }

      virtual void detach(ast::node&, ast::context& context) override {
         _callable->detach(*this, context);
         _args->detach(*this, context);
      }

   private:
      std::unique_ptr<ast::expression> _callable;
      std::unique_ptr<ast::argument_list> _args;
   };
}

namespace rush::ast::exprs {
   inline std::unique_ptr<invoke_expression> invoke(
      std::unique_ptr<ast::expression> callable,
      std::unique_ptr<ast::argument_list> args) {
         return std::make_unique<invoke_expression>(
            std::move(callable),
            std::move(args),
            invoke_expression::factory_tag_t {});
      }
}

#endif // RUSH_AST_EXPRS_INVOCATION_HPP
