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

#ifndef RUSH_AST_TYPE_REF_HPP
#define RUSH_AST_TYPE_REF_HPP

#include "rush/ast/types/type.hpp"

#include <string>
#include <type_traits>

namespace rush::ast {
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
         return dynamic_cast<T const*>(_ptr);
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

#endif // RUSH_AST_TYPE_REF_HPP
