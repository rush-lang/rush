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
#include "rush/ast/stmts/simple.hpp"

#include <memory>

namespace rush::ast {
   namespace decls {
      std::unique_ptr<ast::property_getter_declaration>
      property_get(std::string name, ast::type_ref type, std::unique_ptr<ast::statement> body);

      std::unique_ptr<ast::property_setter_declaration>
      property_set(std::string name, ast::type_ref type, std::unique_ptr<ast::statement> body);
   }

   class property_getter_declaration : public ast::member_declaration {
      struct factory_tag_t {};
      friend std::unique_ptr<ast::property_getter_declaration>
      decls::property_get(std::string name, ast::type_ref type, std::unique_ptr<ast::statement> body);

   public:
      property_getter_declaration(
         std::unique_ptr<ast::function_declaration> func,
         factory_tag_t)
         : _func { std::move(func) }
         { adopt(*_func); }

      ast::function_declaration const& func() const noexcept {
         return *_func;
      }

      virtual ast::declaration_kind kind() const noexcept override {
         return ast::declaration_kind::getter;
      }

      virtual std::string_view name() const noexcept override {
         return _func->name();
      }

      virtual ast::type_ref type() const noexcept override {
         return _func->return_type();
      }

      ast::statement const& body() const noexcept {
         return _func->body();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_property_getter_decl(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         scope.insert(*this);
         scope.push(ast::scope_kind::pseudo);
         attach(scope, *_func);
         scope.pop();
      }

      virtual void detached(ast::context&) override {
         detach(*_func);
      }

   private:
      std::unique_ptr<ast::function_declaration> _func;
   };

   class property_setter_declaration : public ast::member_declaration {
      struct factory_tag_t {};
      friend std::unique_ptr<ast::property_setter_declaration>
      decls::property_set(std::string name, ast::type_ref type, std::unique_ptr<ast::statement> body);

   public:
      property_setter_declaration(
         std::unique_ptr<ast::function_declaration> func,
         factory_tag_t)
         : _func { std::move(func) }
         { adopt(*_func); }

      ast::function_declaration const& func() const noexcept {
         return *_func;
      }

      virtual ast::declaration_kind kind() const noexcept override {
         return ast::declaration_kind::setter;
      }

      virtual std::string_view name() const noexcept override {
         return _func->name();
      }

      virtual ast::type_ref type() const noexcept override {
         return _func->return_type();
      }

      ast::statement const& body() const noexcept {
         return _func->body();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_property_setter_decl(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         scope.insert(*this);
         scope.push(ast::scope_kind::pseudo);
         attach(scope, *_func);
         scope.pop();
      }

      virtual void detached(ast::context&) override {
         detach(*_func);
      }

   private:
      std::unique_ptr<ast::function_declaration> _func;
   };


   namespace decls {
      inline std::unique_ptr<ast::property_getter_declaration>
      property_get(std::string name, std::unique_ptr<ast::statement> body) {
         return property_get(
            std::move(name),
            ast::types::undefined,
            std::move(body));
      }

      inline std::unique_ptr<ast::property_getter_declaration>
      property_get(std::string name, ast::type_ref type, std::unique_ptr<ast::statement> body) {
         return std::make_unique<ast::property_getter_declaration>(
            decls::function(
               std::move(name),
               std::move(type),
               std::move(body)),
            ast::property_getter_declaration::factory_tag_t {});
      }

      inline std::unique_ptr<ast::property_getter_declaration>
      auto_property_get(std::string name) {
         return property_get(
            std::move(name),
            ast::types::undefined,
            stmts::pass());
      }

      inline std::unique_ptr<ast::property_getter_declaration>
      auto_property_get(std::string name, ast::type_ref type) {
         return property_get(
            std::move(name),
            std::move(type),
            stmts::pass());
      }

      inline std::unique_ptr<ast::property_setter_declaration>
      property_set(std::string name, std::unique_ptr<ast::statement> body) {
         return property_set(
            std::move(name),
            ast::types::undefined,
            std::move(body));
      }

      inline std::unique_ptr<ast::property_setter_declaration>
      property_set(std::string name, ast::type_ref type, std::unique_ptr<ast::statement> body) {
         return std::make_unique<ast::property_setter_declaration>(
            decls::function(
               std::move(name),
               std::move(type),
               ptrns::name("value"),
               std::move(body)),
            ast::property_setter_declaration::factory_tag_t {});
      }

      inline std::unique_ptr<ast::property_setter_declaration>
      auto_property_set(std::string name) {
         return property_set(
            std::move(name),
            ast::types::undefined,
            stmts::pass());
      }

      inline std::unique_ptr<ast::property_setter_declaration>
      auto_property_set(std::string name, ast::type_ref type) {
         return property_set(
            std::move(name),
            std::move(type),
            stmts::pass());
      }
   }
} // rush::ast

#endif // RUSH_AST_DECLS_PROPERTY_HPP
