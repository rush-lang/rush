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


namespace rush::ast {
	class constant_declaration;

	namespace decls {
		std::unique_ptr<constant_declaration> constant(
			std::string name,
			ast::type_ref type,
			std::unique_ptr<expression> init);
	}

	class constant_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<constant_declaration>
			decls::constant(std::string, ast::type_ref, std::unique_ptr<expression>);

	public:
		constant_declaration(
         std::string name,
         ast::type_ref type,
         std::unique_ptr<expression> init,
         factory_tag_t)
			: storage_declaration {
				std::move(name),
				std::move(type),
				std::move(init)
			} {}

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::constant;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_constant_decl(*this);
		}
	};

	namespace decls {
		inline std::unique_ptr<constant_declaration> constant(
			std::string name,
			ast::type_ref type,
			std::unique_ptr<expression> init = nullptr
		) {
			return std::make_unique<constant_declaration>(
				std::move(name),
				std::move(type),
				std::move(init),
				constant_declaration::factory_tag_t {});
		}

		inline std::unique_ptr<constant_declaration> constant(
			std::string name,
			std::unique_ptr<expression> init
		) {
			if (!init) throw std::invalid_argument("un-typed constant declaration requires an initializer.");
			return constant(
				std::move(name),
				ast::types::undefined,
				std::move(init));
		}
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_CONSTANT_HPP
