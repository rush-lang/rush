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

#ifndef RUSH_AST_DECLS_NESTED_HPP
#define RUSH_AST_DECLS_NESTED_HPP

#include "rush/ast/decls/member.hpp"
#include "rush/ast/decls/type.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>


namespace rush::ast {
   class nested_declaration;

   namespace decls {
      std::unique_ptr<ast::nested_declaration> nested(
         std::unique_ptr<ast::type_declaration>);
   } // rush::ast::decls

   class nested_declaration : public ast::member_declaration {
      struct factory_tag_t {};
      friend std::unique_ptr<ast::nested_declaration> decls::nested(
         std::unique_ptr<ast::type_declaration>);

   public:
      nested_declaration(
         std::unique_ptr<ast::type_declaration> type,
         factory_tag_t)
         : _decl { std::move(type) }
         { adopt(*_decl); }

      virtual ast::declaration_kind kind() const noexcept override {
         return _decl->kind();
      }

      virtual ast::type_ref type() const noexcept override {
         return _decl->type();
      }

      virtual std::string_view name() const noexcept override {

         return _decl->name();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         _decl->accept(v);
      }

   protected:
      virtual void adopted(ast::node& parent) override {
         orphan(*_decl);
         adopt(*_decl, &parent);
      }

      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_decl);
      }

      virtual void detached(ast::context&) override {
         detach(*_decl);
      }

   private:
      std::unique_ptr<ast::type_declaration> _decl;
   };

   namespace decls {
      inline std::unique_ptr<ast::nested_declaration> nested(
         std::unique_ptr<ast::type_declaration> type) {
            return std::make_unique<ast::nested_declaration>(std::move(type),
               ast::nested_declaration::factory_tag_t {});
         }
   } // decls
} // rush::ast

#endif // RUSH_AST_DECLS_NESTED_HPP
