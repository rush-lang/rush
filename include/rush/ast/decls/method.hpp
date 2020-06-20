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

#ifndef RUSH_AST_DECLS_METHOD_HPP
#define RUSH_AST_DECLS_METHOD_HPP

#include "rush/ast/decls/member.hpp"
#include "rush/ast/decls/function.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>


namespace rush::ast {
   class method_declaration;

   namespace decls {
      std::unique_ptr<ast::method_declaration> method(
         std::unique_ptr<ast::function_declaration>);
   } // rush::ast::decls

   class method_declaration : public ast::member_declaration {
      struct factory_tag_t {};
      friend std::unique_ptr<ast::method_declaration> decls::method(
         std::unique_ptr<ast::function_declaration>);

   public:
      method_declaration(
         std::unique_ptr<ast::function_declaration> func,
         factory_tag_t)
         : _decl { std::move(func) } {}

      virtual ast::declaration_kind kind() const noexcept override {
         return ast::declaration_kind::method;
      }

      virtual ast::type_ref type() const noexcept override {
         return _decl->type();
      }

      virtual std::string_view name() const noexcept override {
         return _decl->name();
      }

      ast::type_ref return_type() const noexcept {
         return _decl->return_type();
      }

      ast::pattern const& parameters() const noexcept {
         return _decl->parameters();
      }

      ast::statement const& body() const noexcept {
         return _decl->body();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_method_decl(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_decl);
      }

      virtual void detached(ast::context&) override {
         detach(*_decl);
      }

   private:
      std::unique_ptr<ast::function_declaration> _decl;
   };

   namespace decls {
      inline std::unique_ptr<ast::method_declaration> method(
         std::unique_ptr<ast::function_declaration> func) {
            return std::make_unique<ast::method_declaration>(std::move(func),
               ast::method_declaration::factory_tag_t {});
         }

      inline std::unique_ptr<ast::method_declaration> method(
			std::string name,
         ast::type_ref return_type,
			std::unique_ptr<ast::pattern> params,
			std::unique_ptr<ast::statement> body) {
            return decls::method(
               decls::function(std::move(name),
                  std::move(return_type),
                  std::move(params),
                  std::move(body)));
			}

      inline std::unique_ptr<ast::method_declaration> method(
			std::string name,
			std::unique_ptr<ast::statement> body) {
				return decls::method(
					std::move(name),
               ast::types::undefined,
               ptrns::list(),
					std::move(body));
			}

		inline std::unique_ptr<ast::method_declaration> method(
			std::string name,
			ast::type_ref return_type,
			std::unique_ptr<ast::statement> body) {
				return decls::method(
					std::move(name),
					std::move(return_type),
               ptrns::list(),
					std::move(body));
			}

		inline std::unique_ptr<ast::method_declaration> method(
			std::string name,
			std::unique_ptr<ast::pattern> params,
			std::unique_ptr<ast::statement> body) {
				return decls::method(
					std::move(name),
               ast::types::undefined,
					std::move(params),
					std::move(body));
			}
   } // decls
} // rush::ast

#endif // RUSH_AST_DECLS_METHOD_HPP
