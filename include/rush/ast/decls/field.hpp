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

#ifndef RUSH_AST_DECLS_FIELD_HPP
#define RUSH_AST_DECLS_FIELD_HPP

#include "rush/ast/decls/member.hpp"
#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

#include <memory>

namespace rush::ast {
   class variable_field_declaration;
   class constant_field_declaration;

   namespace decls {
      std::unique_ptr<ast::variable_field_declaration> field(
         std::unique_ptr<ast::variable_declaration>);

      std::unique_ptr<ast::constant_field_declaration> field(
         std::unique_ptr<ast::constant_declaration>);
   } // rush::ast::decls


   class field_declaration : public ast::member_declaration {
      struct factory_tag_t {};

      friend class variable_field_declaration;
      friend class constant_field_declaration;

   public:
      field_declaration(
         std::unique_ptr<ast::storage_declaration> decl,
         factory_tag_t)
         : _decl { std::move(decl) } {}

      virtual declaration_kind kind() const noexcept override {
			return declaration_kind::field;
		}

      ast::pattern& pattern() const noexcept {
         return _decl->pattern();
      }

      ast::type_ref type() const noexcept override {
         return ast::types::undefined;
      }

      std::string_view name() const noexcept override {
         return "";
      }

      auto names() const {
         return _decl->names();
      }

      auto annotations() const {
         return _decl->annotations();
      }

      auto initializers() const {
         return _decl->initializers();
      }

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         attach(*_decl);
      }

      virtual void detached(ast::node*, ast::context&) override {
         detach(*_decl);
      }

   private:
      std::unique_ptr<ast::storage_declaration> _decl;
   };


   class variable_field_declaration : public ast::field_declaration {
      struct factory_tag_t {};

      friend std::unique_ptr<ast::variable_field_declaration> decls::field(
         std::unique_ptr<ast::variable_declaration>);

   public:
      variable_field_declaration(
         std::unique_ptr<ast::variable_declaration> decl,
         factory_tag_t)
         : ast::field_declaration {
            std::move(decl),
            ast::field_declaration::factory_tag_t {} } {}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_variable_field_decl(*this);
      }
   };


   class constant_field_declaration : public ast::field_declaration {
      struct factory_tag_t {};

      friend std::unique_ptr<ast::constant_field_declaration> decls::field(
         std::unique_ptr<ast::constant_declaration>);

   public:
      constant_field_declaration(
         std::unique_ptr<ast::constant_declaration> decl,
         factory_tag_t)
         : ast::field_declaration {
            std::move(decl),
            ast::field_declaration::factory_tag_t {} } {}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_constant_field_decl(*this);
      }
   };

   namespace decls {
      inline std::unique_ptr<ast::variable_field_declaration> field(
         std::unique_ptr<ast::variable_declaration> decl) {
            return std::make_unique<ast::variable_field_declaration>(
               std::move(decl),
               ast::variable_field_declaration::factory_tag_t {});
         }

      inline std::unique_ptr<ast::constant_field_declaration> field(
         std::unique_ptr<ast::constant_declaration> decl) {
            return std::make_unique<ast::constant_field_declaration>(
               std::move(decl),
               ast::constant_field_declaration::factory_tag_t {});
         }
   } // rush::ast::decls

} // rush::ast

#endif // RUSH_AST_DECLS_FIELD_HPP
