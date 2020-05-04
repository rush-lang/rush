#pragma once

#ifndef RUSH_AST_TYPES_FUNCTION_HPP
#define RUSH_AST_TYPES_FUNCTION_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
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
      function_type(ast::type_ref return_type, ast::type_ref param_types)
         : _param_types { std::move(param_types) }
         , _return_type { std::move(return_type) } {}

      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::function;
      }

      ast::type_ref return_type() const noexcept {
         return _return_type;
      }

      ast::type_ref const& parameter_types() const noexcept {
         return _param_types;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_function_type(*this);
      }

   private:
      ast::type_ref _param_types;
      ast::type_ref _return_type;
   };
}

#endif // RUSH_AST_TYPES_FUNCTION_HPP
