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

   protected:
      virtual void attached(ast::scope&, ast::context&) override {}
      virtual void detached(ast::context&) override {}
   };
} // rush::ast

#endif // RUSH_AST_TYPES_STRING_HPP
