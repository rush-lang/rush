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

#ifndef RUSH_AST_TYPES_EXTENSION_HPP
#define RUSH_AST_TYPES_EXTENSION_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/type_ref.hpp"

namespace rush::ast {
   class type_extension;

   template <>
   struct type_traits<ast::type_extension> {
      static constexpr ast::type_kind kind() noexcept {
         return ast::type_kind::extension;
      }
   };

   class type_extension : public ast::type {
   public:
      type_extension(ast::type_ref type)
         : _type { std::move(type) } {}

      virtual ast::type_kind kind() const override {
         return type_traits<type_extension>::kind();
      }

      ast::type_ref underlying_type() const noexcept {
         return _type;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_type_extension(*this);
      }

   protected:
      virtual void detached(ast::context&) override {}
      virtual void attached(ast::scope&, ast::context&) override {}

   private:
      ast::type_ref _type;
   };
} // rush::ast

#endif // RUSH_AST_TYPES_EXTENSION_HPP
