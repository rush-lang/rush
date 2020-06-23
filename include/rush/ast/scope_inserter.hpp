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

#ifndef RUSH_AST_SCOPE_INSERTER_HPP
#define RUSH_AST_SCOPE_INSERTER_HPP

#include "rush/ast/scope.hpp"
#include "rush/ast/traversal.hpp"
#include "rush/ast/ptrns/named.hpp"
#include "rush/ast/decls/class.hpp"
#include "rush/ast/decls/struct.hpp"
#include "rush/ast/decls/function.hpp"
#include "rush/ast/decls/import.hpp"

#include <iostream>


namespace rush::ast {
   class scope_inserter : public ast::traversal {
   public:
      scope_inserter(ast::scope& sc)
         : _scope { sc } {}

      void visit_named_ptrn(ast::named_pattern const& ptrn) {
         _scope.insert(ptrn);
      }

      void visit_function_decl(ast::function_declaration const& decl) {
         _scope.insert(decl);
      }

      void visit_class_decl(ast::class_declaration const& decl) {
         _scope.insert(decl);
      }

      void visit_struct_decl(ast::struct_declaration const& decl) {
         _scope.insert(decl);
      }

      void visit_method_decl(ast::method_declaration const& decl) {
         if (!decl.is_constructor()) {
            _scope.insert(decl); }
      }

      void visit_import_decl(ast::import_declaration const& decl) {
         // TODO: insert module names as scope identifiers.
      }

   private:
      ast::scope& _scope;
   };
} // rush::ast

#endif // RUSH_AST_SCOPE_INSERTER_HPP
