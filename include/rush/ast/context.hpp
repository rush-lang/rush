/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#pragma once

#ifndef RUSH_AST_CONTEXT_HPP
#define RUSH_AST_CONTEXT_HPP

#include "rush/extra/iterator_range.hpp"
#include "rush/extra/hash_combine.hpp"
#include "rush/ast/types/type_resolver.hpp"
#include "rush/ast/types.hpp"

#include <vector>
#include <numeric>
#include <functional>
#include <unordered_map>
#include <algorithm>

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
      rush::iterator_range<std::vector<ast::type_ref>::const_iterator> types;
      bool operator ==(tuple_type_key_t const& other) const {
         return std::equal(
            this->types.begin(), this->types.end(),
            other.types.begin(), other.types.end());
      }
   };

   struct function_type_key_t {
      ast::type_ref ret_type;
      ast::type_ref params_type;
      bool operator ==(function_type_key_t const& other) const {
         return this->ret_type == other.ret_type
             && this->params_type == other.params_type;
      }
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

   template <>
   struct hash<rush::ast::detail::tuple_type_key_t> {
      std::size_t operator()(rush::ast::detail::tuple_type_key_t const& key) const {
         std::hash<rush::ast::type_ref> hasher;
         return std::accumulate(
            key.types.begin(),
            key.types.end(), 0,
            [&hasher](auto& result, auto& type) {
               return result ^ hasher(type) + 0x9e3779b9 + (result<<6) + (result>>2);
            });
      }
   };

   template <>
   struct hash<rush::ast::detail::function_type_key_t> {
      std::size_t operator()(rush::ast::detail::function_type_key_t const& key) const {
         std::size_t result = 0;
         rush::hash_combine(result, key.ret_type);
         rush::hash_combine(result, key.params_type);
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
      ast::type_ref array_type(ast::array_literal_expression&);

      ast::type_ref tuple_type(ast::type_ref single);
      ast::type_ref tuple_type(ast::tuple_literal_expression&);
      ast::type_ref tuple_type(rush::iterator_range<std::vector<ast::type_ref>::const_iterator>);

      ast::type_ref function_type(ast::type_ref ret, ast::type_ref params);
      std::variant<ast::type_ref, ast::type_resolver*> function_type(ast::lambda_expression const&);
      std::variant<ast::type_ref, ast::type_resolver*> function_type(ast::function_declaration const&);

      ast::type_ref optional_type(ast::type_ref);
      ast::type_ref type_extension(ast::type_ref);

   private:
      std::unordered_map<ast::node const*, std::unique_ptr<ast::type_resolver>> _type_resolvers;
      std::unordered_map<ast::type_ref, std::unique_ptr<ast::optional_type>> _optional_types;
      std::unordered_map<ast::type_ref, std::unique_ptr<ast::type_extension>> _type_extensions;
      std::unordered_map<detail::array_type_key_t, std::unique_ptr<ast::array_type>> _array_types;
      std::unordered_map<detail::tuple_type_key_t, std::unique_ptr<ast::tuple_type>> _tuple_types;
      std::unordered_map<detail::function_type_key_t, std::unique_ptr<ast::function_type>> _function_types;

      std::variant<ast::type_ref, ast::type_resolver*> function_type_impl(ast::node const* node, ast::type_ref ret, ast::type_ref params);

      class function_type_resolver_callback;
   };
}

#endif // RUSH_AST_CONTEXT_HPP
