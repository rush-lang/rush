#pragma once

#ifndef RUSH_AST_TYPES_ARRAY_HPP
#define RUSH_AST_TYPES_ARRAY_HPP

#include "rush/ast/types/type.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace rush::ast {
   class array_type;
   class array_type_map;

   template <>
   struct type_traits<array_type> {
      static constexpr ast::type_kind kind() noexcept {
         return ast::type_kind::array;
      }
   };

   class array_type_dim {
   public:
      using size_type = std::size_t;
      array_type_dim(size_type sz) : _size { sz } {}
      size_type size() const noexcept { return _size; }

   private:
      size_type _size;
   };

   class array_type : public ast::type {
      friend class array_type_map;
   public:
      array_type(ast::type_ref type, std::vector<array_type_dim> dims)
         : _dims { std::move(dims) }
         , _type { std::move(type) } {}

      virtual ast::type_kind kind() const override {
         return type_traits<array_type>::kind();
      }

      std::size_t rank() const noexcept {
         return _dims.size();
      }

      std::vector<array_type_dim> const& dimensions() const noexcept {
         return _dims;
      }

      ast::type_ref underlying_type() const noexcept {
         return _type;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_array_type(*this);
      }

      virtual void attach(ast::node&, ast::context&) override {}
      virtual void detach(ast::node&, ast::context&) override {}

   private:
      ast::type_ref _type;
      std::vector<array_type_dim> _dims;
   };

   // Container class for storage and retrieval of array types.
   class array_type_map {
   public:
      ast::array_type const& get(ast::type_ref type);
      ast::array_type const& get(ast::type_ref type, std::vector<array_type_dim> dims);

   private:
      std::unordered_map<ast::type_ref, array_type> _types;
   };
} // rush::ast

#endif // RUSH_AST_TYPES_ARRAY_HPP
