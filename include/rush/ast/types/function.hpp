#pragma once

#ifndef RUSH_AST_TYPES_FUNCTION_HPP
#define RUSH_AST_TYPES_FUNCTION_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/named.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/list.hpp"
#include "rush/core/iterator_range.hpp"

#include <vector>
#include <memory>

namespace rush::ast {
   class function_type : public node {
   public:
      function_type(ast::type return_type, ast::parameter_list params)
         : _params { std::move(params) }
         , _return_type { std::move(return_type) } {}

      ast::type return_type() const noexcept {
         return _return_type;
      }

      ast::parameter_list const& parameters() const noexcept {
         return _params;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) {
         // v.visit_function_type(*this);
      }

   private:
      ast::parameter_list _params;
      ast::type _return_type;
   };

   namespace types {
      inline std::unique_ptr<function_type> function(
         ast::type return_type,
         parameter_list params) {
            return nullptr;
            // return function(return_type, ast::parameter)
         }

      inline std::unique_ptr<function_type> function(
         ast::type return_type,
         rush::iterator_range<ast::type> param_types) {
            return nullptr;
         }
   }
}

#endif // RUSH_AST_TYPES_FUNCTION_HPP
