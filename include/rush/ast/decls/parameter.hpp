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

#ifndef RUSH_AST_DECLS_PARAMETER_HPP
#define RUSH_AST_DECLS_PARAMETER_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/decls/storage.hpp"
#include "rush/ast/ptrns/named.hpp"
#include "rush/ast/ptrns/binding.hpp"
#include "rush/ast/ptrns/type_annotation.hpp"

#include <memory>

namespace rush::ast {
	class parameter_declaration;

	namespace decls {
		std::unique_ptr<parameter_declaration> parameter(
         std::unique_ptr<ast::pattern> patt);
	}

	class parameter_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<parameter_declaration>
			decls::parameter(std::unique_ptr<ast::pattern>);

	public:
		parameter_declaration(std::unique_ptr<ast::pattern> patt, factory_tag_t)
			: storage_declaration { std::move(patt) } {}

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::parameter;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_parameter_decl(*this);
		}
	};

	namespace decls {
      inline std::unique_ptr<ast::parameter_declaration>
         parameter(std::unique_ptr<ast::pattern> patt) {
            return std::make_unique<ast::parameter_declaration>(
               std::move(patt),
               ast::parameter_declaration::factory_tag_t {});
         }

      inline std::unique_ptr<ast::parameter_declaration>
         parameter(std::string name, ast::type_ref type) {
            return parameter(ptrns::annotation(
               ptrns::name(std::move(name)),
               type));
         }

      inline std::unique_ptr<ast::parameter_declaration>
         parameter(std::string name, std::unique_ptr<ast::expression> default_) {
            return parameter(ptrns::binding(
               ptrns::name(std::move(name)),
               std::move(default_)));
         }

      inline std::unique_ptr<ast::parameter_declaration>
         parameter(std::string name, ast::type_ref type, std::unique_ptr<ast::expression> default_) {
            return parameter(ptrns::binding(
               ptrns::annotation(
                  ptrns::name(std::move(name)),
                  std::move(type)),
               std::move(default_)));
       }
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_PARAMETER_HPP
