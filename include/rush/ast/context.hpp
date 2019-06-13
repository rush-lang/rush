#pragma once

#ifndef RUSH_AST_CONTEXT_HPP
#define RUSH_AST_CONTEXT_HPP

#include "rush/extra/iterator_range.hpp"
#include "rush/extra/hash_combine.hpp"
#include "rush/ast/types.hpp"

#include <functional>
#include <unordered_map>

namespace rush::ast::detail {
   struct array_type_key_t {
      std::size_t rank;
      ast::type_ref type;
      bool operator ==(array_type_key_t const& other) const {
         return rank == other.rank
             && type == other.type;
      }
   };

   struct tuple_type_key_t {
      iterator_range<ast::type_ref> types;
   };

   struct function_type_key_t {
      ast::type_ref ret_type;
      tuple_type_key_t params;
   };
}

namespace std {
   template <>
   struct hash<rush::ast::detail::array_type_key_t> {
      std::size_t operator()(rush::ast::detail::array_type_key_t const& key) const {
         std::size_t result = 0;
         rush::hash_combine(result, key.rank);
         rush::hash_combine(result, key.type);
         return result;
      }
   };
}

namespace rush::ast {
   class context {
      context(context const&) = delete;
      void operator =(context const&) = delete;

   public:
      using size_type = std::size_t;

      context() = default;

      ast::type_ref bool_type();
      ast::type_ref char_type();
      ast::type_ref string_type();

      ast::type_ref ieee32_type();
      ast::type_ref ieee64_type();

      ast::type_ref int8_type();
      ast::type_ref int16_type();
      ast::type_ref int32_type();
      ast::type_ref int64_type();

      ast::type_ref uint8_type();
      ast::type_ref uint16_type();
      ast::type_ref uint32_type();
      ast::type_ref uint64_type();

      ast::type_ref signed_integral_type(integral_kind);
      ast::type_ref unsigned_integral_type(integral_kind);
      ast::type_ref floating_point_type(floating_point_kind);

      ast::type_ref array_type(ast::type_ref, size_type = 1);
      ast::type_ref tuple_type(iterator_range<ast::type_ref>);
      ast::type_ref function_type(ast::type_ref ret, iterator_range<ast::type_ref> params);

   private:
      std::unordered_map<detail::array_type_key_t, std::unique_ptr<ast::array_type>> _array_types;
      // std::unordered_map<detail::tuple_type_key_t, ast::tuple_type> _tuple_types;
      // std::unordered_map<detail::function_type_key_t, ast::function_type> _function_types;
   };
}

#endif // RUSH_AST_CONTEXT_HPP
