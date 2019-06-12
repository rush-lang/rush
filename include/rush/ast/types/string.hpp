#pragma once

#ifndef RUSH_AST_TYPES_STRING_HPP
#define RUSH_AST_TYPES_STRING_HPP

#include "rush/ast/types/type.hpp"

namespace rush::ast {
   class builtin_string_type : public type {
   public:
      virtual ast::type_kind kind() const override {
         return ast::type_kind::builtin_string;
      };

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_builtin_string_type(*this);
      }

      virtual void attach(ast::node&, ast::context&) override {}
      virtual void detach(ast::node&, ast::context&) override {}
   };
} // rush::ast

#endif // RUSH_AST_TYPES_STRING_HPP
