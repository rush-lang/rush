#pragma once

#ifndef RUSH_AST_TYPES_FUNCTION_HPP
#define RUSH_AST_TYPES_FUNCTION_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/named.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/list.hpp"

#include <vector>
#include <memory>

namespace rush::ast::decls {
   std::unique_ptr<function_declaration> function(
      std::string,
      ast::type_ref,
      std::unique_ptr<parameter_list>,
      std::unique_ptr<statement>);
}

namespace rush::ast {
   class function_type : public ast::type {
      struct factory_tag_t {};

      friend std::unique_ptr<function_declaration> decls::function(
			std::string,
         ast::type_ref,
			std::unique_ptr<parameter_list>,
			std::unique_ptr<statement>);

   public:
      function_type(ast::type_ref return_type, std::unique_ptr<ast::parameter_list> params, factory_tag_t)
         : _params { std::move(params) }
         , _return_type { std::move(return_type) } {}

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

   private:
      ast::type_ref _return_type;
      std::unique_ptr<ast::parameter_list> _params;
   };
}

#endif // RUSH_AST_TYPES_FUNCTION_HPP
