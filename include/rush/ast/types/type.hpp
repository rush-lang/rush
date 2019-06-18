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
   public:
      virtual ast::type_kind kind() const = 0;

      std::string to_string() const;
   };

   //! Value object that references an actual instance of rush::ast::type.
	class type_ref {
   public:
      type_ref(ast::type const& ref)
         : _ptr { std::addressof(ref) } {}

      ast::type_kind kind() const noexcept {
         return _ptr->kind();
      }

      std::string to_string() const {
         return _ptr->to_string();
      }

      ast::type const* get() const noexcept {
         return _ptr;
      }

      template <typename T>
      bool is() const noexcept {
         static_assert(std::is_base_of_v<ast::type, T>, "T is not a dervied rush::ast::type");
         return type_traits<T>::kind() == _ptr->kind();
      }

      template <typename T>
      T const* as() const noexcept {
         static_assert(std::is_base_of_v<ast::type, T>, "T is not a derived rush::ast::type");
         return reinterpret_cast<T const*>(_ptr);
      }

      void accept(ast::visitor& v) const {
         _ptr->accept(v);
      }

      void accept(ast::visitor&& v) const {
         _ptr->accept(std::move(v));
      }

      friend bool operator == (ast::type_ref const& lhs, ast::type_ref const& rhs) {
         return lhs._ptr == rhs._ptr;
      }

      friend bool operator != (ast::type_ref const& lhs, ast::type_ref const& rhs) {
         return !(operator ==(lhs, rhs));
      }

   private:
      ast::type const* _ptr;
   };
}

namespace std {
   template <>
   struct hash<rush::ast::type_ref> {
      size_t operator()(rush::ast::type_ref const& x) const {
         auto fn = hash<decltype(x.get())>();
         return fn(x.get());
      }
   };
}

#endif // RUSH_AST_TYPE_HPP
