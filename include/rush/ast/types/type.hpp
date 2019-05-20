#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>
#include <string>
#include <variant>

namespace rush::ast {
   class named_type;
   class array_type;
   class tuple_type;
   class builtin_type;
   class function_type;


   // Value object that stores a pointer to an actual type, named or unnamed.
	class type {
		template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
		template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

   public:
      type(std::unique_ptr<ast::builtin_type> const& ty)
         : _ptr { ty.get() } {}

      type(std::unique_ptr<ast::array_type> const& ty)
         : _ptr { ty.get() } {}

      type(std::unique_ptr<ast::tuple_type> const& ty)
         : _ptr { ty.get() } {}

      type(std::unique_ptr<ast::function_type> const& ty)
         : _ptr { ty.get() } {}

      void accept(ast::visitor& v) {
         // std::visit(overloaded {
         //    [&v](ast::named_type const* t) { v.visit_named_type(*t); },
         //    [&v](ast::unnamed_type const* t) { v.visit_unnamed_type(*t); },
         // }, _ptr);
      }

      void accept(ast::visitor&& v) {
         // std::visit(overloaded {
         //    [&v](ast::named_type const* t) { v.visit_named_type(*t); },
         //    [&v](ast::unnamed_type const* t) { v.visit_unnamed_type(*t); },
         // }, _ptr);
      }

   private:
      std::variant<
         ast::named_type const*,
         ast::builtin_type const*,
         ast::array_type const*,
         ast::tuple_type const*,
         ast::function_type const*> _ptr;
   };
}

#endif // RUSH_AST_TYPE_HPP
