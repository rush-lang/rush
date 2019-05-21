#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>

namespace rush::ast {
   class named_type;
   class array_type;
   class tuple_type;
   class builtin_type;
   class function_type;

   enum class type_kind {
      builtin,
      array,
      tuple,
      function
   };

   class type : public node {
   public:
      virtual type_kind kind() const = 0;
   };

   // Value object that stores a pointer to an actual type, named or unnamed.
	class type_ref {
   public:
      type_ref(ast::type const& ref)
         : _ptr { std::addressof(ref) } {}

      ast::type_kind kind() const noexcept {
         return _ptr->kind();
      }

      void accept(ast::visitor& v) const {
         _ptr->accept(v);
      }

      void accept(ast::visitor&& v) const {
         _ptr->accept(std::move(v));
      }

   private:
      ast::type const* _ptr;
   };
}

#endif // RUSH_AST_TYPE_HPP
