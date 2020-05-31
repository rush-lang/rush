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

#ifndef RUSH_AST_DECLS_PROPERTY_HPP
#define RUSH_AST_DECLS_PROPERTY_HPP

#include "rush/ast/decls/member.hpp"
#include "rush/ast/decls/function.hpp"

#include <memory>

namespace rush::ast {

   class property_getter_declaration : public ast::member_declaration {
   public:
      using node::accept;
      virtual void accept(ast::visitor& v) {
         v.visit_property_getter_decl(*this);
      }
   private:
      std::unique_ptr<ast::function_declaration> _func;
   };

   class property_setter_declaration : public ast::member_declaration {
   public:
      using node::accept;
      virtual void accept(ast::visitor& v) {
         v.visit_property_setter_decl(*this);
      }
   private:
      std::unique_ptr<ast::function_declaration> _func;
   };

} // rush::ast

#endif // RUSH_AST_DECLS_PROPERTY_HPP
