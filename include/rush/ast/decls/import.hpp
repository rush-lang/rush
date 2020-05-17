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

#ifndef RUSH_AST_DECLS_IMPORT_HPP
#define RUSH_AST_DECLS_IMPORT_HPP

#include "rush/ast/decls/nominal.hpp"
#include "rush/ast/types/builtin.hpp"

#include <memory>

namespace rush::ast {
   class import_declaration : public ast::nominal_declaration {
   public:
      import_declaration(std::string name)
         : _name { std::move(name) } {}

      virtual std::string_view name() const override {
         return _name;
      }

      virtual ast::type_ref type() const override {
         return ast::types::undefined;
      }

		virtual declaration_kind kind() const override {
         return ast::declaration_kind::module;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_import_decl(*this);
      }

      virtual void attach(ast::node&, ast::context&) override {}
      virtual void detach(ast::node&, ast::context&) override {}

   private:
      std::string _name;
   };

   namespace decls {
      inline std::unique_ptr<ast::import_declaration> import(std::string name) {
         return std::make_unique<ast::import_declaration>(std::move(name));
      }
   }
}

#endif // RUSH_AST_DECLS_IMPORT_HPP
