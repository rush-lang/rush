#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <string>
#include <variant>

namespace rush::ast {
   class named_type;
   class unnamed_type;

   // Value object that stores a pointer to an actual type, named or unnamed.
	class type {
		template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
		template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

   public:
      type(ast::named_type const& ty)
         : _ptr { std::addressof(ty) } {}

      type(ast::unnamed_type const& ty)
         : _ptr { std::addressof(ty) } {}

      template <typename Type>
      type(std::unique_ptr<Type> const& pty)
         : _ptr { pty.get() } {}

      void accept(ast::visitor& v) {
         std::visit(overloaded {
            [&v](ast::named_type const* t) { v.visit_named_type(*t); },
            [&v](ast::unnamed_type const* t) { v.visit_unnamed_type(*t); },
         }, _ptr);
      }

      void accept(ast::visitor&& v) {
         std::visit(overloaded {
            [&v](ast::named_type const* t) { v.visit_named_type(*t); },
            [&v](ast::unnamed_type const* t) { v.visit_unnamed_type(*t); },
         }, _ptr);
      }

   private:
      std::variant<
         ast::named_type const*,
         ast::unnamed_type const*> _ptr;
   };
}

#endif // RUSH_AST_TYPE_HPP
