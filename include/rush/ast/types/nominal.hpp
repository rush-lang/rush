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

#ifndef RUSH_AST_TYPES_NOMINAL_HPP
#define RUSH_AST_TYPES_NOMINAL_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace rush::ast {
   class scope;
   class context;

   class named_type : public ast::type {
   public:
      named_type(std::string name)
         : _name { std::move(name) } {}

      std::string_view name() const noexcept {
         return _name;
      }

      virtual ast::type_kind kind() const override {
         return ast::type_kind::error;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_named_type(*this);
		}

   private:
      std::string _name;
   };

   ast::type_ref resolve_named_types(ast::type_ref, ast::scope&, ast::context&);
} // rush::ast

#endif // RUSH_AST_TYPES_NOMINAL_HPP
