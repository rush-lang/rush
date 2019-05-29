#pragma once

#ifndef RUSH_AST_TYPES_REDUCER_HPP
#define RUSH_AST_TYPES_REDUCER_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/utility.hpp"
#include "rush/ast/visitor.hpp"

namespace rush::ast::types {
   class type_comparer : ast::visitor {
   public:
      virtual void visit_builtin_type() {

      }

   private:
      ast::type_ref _lhs;
   };

   class type_reducer {
   public:
      type_reducer(ast::type_ref first)
         : _first { first } {}

      ast::type_ref operator ()(ast::type_ref second) const noexcept {
         if (_first == second)
            return _first;

         return types::error_type;
      }

   private:
      ast::type_ref _first;
   };
}


#endif // RUSH_AST_TYPES_REDUCER_HPP
