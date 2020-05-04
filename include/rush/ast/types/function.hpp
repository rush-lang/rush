#pragma once

#ifndef RUSH_AST_TYPES_FUNCTION_HPP
#define RUSH_AST_TYPES_FUNCTION_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/decls/parameter.hpp"

#include <vector>
#include <memory>
#include <optional>

namespace rush::ast::decls {
   std::unique_ptr<function_declaration> function(
      std::string,
      ast::type_ref,
      std::unique_ptr<parameter_list>,
      std::unique_ptr<statement>);
}

namespace rush::ast::exprs {
   std::unique_ptr<lambda_expression> lambda(
      std::unique_ptr<ast::parameter_list>,
      std::unique_ptr<ast::statement>);
}

namespace rush::ast {
   class function_declaration;

   class function_type : public ast::type {
      friend class lambda_expression;
      friend class function_declaration;
      struct factory_tag_t {};

      friend std::unique_ptr<lambda_expression> exprs::lambda(
			std::unique_ptr<ast::parameter_list>,
			std::unique_ptr<ast::statement>);

      friend std::unique_ptr<function_declaration> decls::function(
         std::string,
         ast::type_ref,
         std::unique_ptr<parameter_list>,
         std::unique_ptr<statement>);

   public:
      function_type(ast::type_ref return_type, std::unique_ptr<ast::parameter_list> params, factory_tag_t)
         : _params { std::move(params) }
         , _return_type { std::move(return_type) }
         , _resolve_iter { 0 } {}

      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::function;
      }

      ast::type_ref return_type() const noexcept {
         return _return_type;
      }

      ast::parameter_list const& parameters() const noexcept {
         return *_params;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_function_type(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}

   private:
      std::unique_ptr<ast::parameter_list> _params;
      mutable ast::type_ref _return_type;
      mutable int _resolve_iter;

      void resolve_return_type(ast::statement const&) const;
   };
}

#endif // RUSH_AST_TYPES_FUNCTION_HPP
