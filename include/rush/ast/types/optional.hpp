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

#ifndef RUSH_AST_TYPES_OPTIONAL_HPP
#define RUSH_AST_TYPES_OPTIONAL_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/type_ref.hpp"

namespace rush::ast {
   class optional_type;

   template <>
   struct type_traits<ast::optional_type> {
      static constexpr ast::type_kind kind() noexcept {
         return ast::type_kind::optional;
      }
   };

   class optional_type : public ast::type {
   public:
      optional_type(ast::type_ref type)
         : _type { std::move(type) } {}

      virtual ast::type_kind kind() const override {
         return type_traits<optional_type>::kind();
      }

      ast::type_ref underlying_type() const noexcept {
         return _type;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_optional_type(*this);
      }

   protected:
      virtual void attached(ast::scope&, ast::context&) override {}
      virtual void detached(ast::context&) override {}

   private:
      ast::type_ref _type;
   };
} // rush::ast

#endif // RUSH_AST_TYPES_OPTIONAL_HPP
