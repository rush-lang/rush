#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>
#include <string>
#include <type_traits>

namespace rush::ast {
   enum class type_kind {
      error,
      builtin_void,
      builtin_bool,
      builtin_char,
      builtin_string,
      builtin_integral,
      builtin_floating_point,
      array,
      tuple,
      function
   };

   template <typename TypeT>
   struct type_traits {};

   class type : public node {
      friend class type_ref;
   public:
      virtual ast::type_kind kind() const = 0;

      std::string to_string() const;

      virtual void attach(ast::node&, ast::context&) override {};
      virtual void detach(ast::node&, ast::context&) override {};
   };
}

#endif // RUSH_AST_TYPE_HPP
