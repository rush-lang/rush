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

#ifndef RUSH_AST_DECLS_CONSTANT_HPP
#define RUSH_AST_DECLS_CONSTANT_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/decls/storage.hpp"
#include "rush/ast/ptrns/named.hpp"
#include "rush/ast/ptrns/binding.hpp"
#include "rush/ast/ptrns/type_annotation.hpp"

#include <memory>

namespace rush::ast {
	class constant_declaration;

	namespace decls {
		std::unique_ptr<constant_declaration>
         let(std::unique_ptr<ast::pattern>);
	}

	class constant_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<constant_declaration>
			decls::let(std::unique_ptr<ast::pattern>);

	public:
		constant_declaration(
         std::unique_ptr<ast::pattern> ptrn,
         factory_tag_t)
			: storage_declaration { std::move(ptrn) } {}

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::constant;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_constant_decl(*this);
		}
	};

	namespace decls {
      inline std::unique_ptr<ast::constant_declaration>
         let(std::unique_ptr<ast::pattern> ptrn) {
            return std::make_unique<ast::constant_declaration>(
               std::move(ptrn),
               ast::constant_declaration::factory_tag_t {});
         }

      inline std::unique_ptr<ast::constant_declaration>
         let(std::string name, ast::type_ref type) {
            return let(ptrns::annotation(
               ptrns::name(std::move(name)),
               type));
         }

      inline std::unique_ptr<ast::constant_declaration>
         let(std::string name, std::unique_ptr<ast::expression> init) {
            return let(ptrns::binding(
               ptrns::name(std::move(name)),
               std::move(init)));
         }

      inline std::unique_ptr<ast::constant_declaration>
         let(std::string name, ast::type_ref type, std::unique_ptr<ast::expression> init) {
            return let(ptrns::binding(
               ptrns::annotation(
                  ptrns::name(std::move(name)),
                  std::move(type)),
               std::move(init)));
         }
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_CONSTANT_HPP
