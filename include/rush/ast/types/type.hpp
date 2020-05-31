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
      enum_,
      class_,
      struct_,
      concept,
      interface,
      function,
      extension,
      optional,
   };

   template <typename TypeT>
   struct type_traits {};

   class type : public virtual node {
      friend class type_ref;
   public:
      virtual ast::type_kind kind() const = 0;

      std::string to_string() const;

   protected:
      virtual void attached(ast::node*, ast::context&) override {};
      virtual void detached(ast::node*, ast::context&) override {};
   };
}

#endif // RUSH_AST_TYPE_HPP
