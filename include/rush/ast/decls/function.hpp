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

#ifndef RUSH_AST_DECLS_FUNCTION_HPP
#define RUSH_AST_DECLS_FUNCTION_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/ptrns/list.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/function.hpp"
#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/decls/nominal.hpp"

#include "rush/ast/context.hpp"


namespace rush::ast::decls {
   std::unique_ptr<function_declaration> function(
      std::string,
      ast::type_ref,
      std::unique_ptr<pattern>,
      std::unique_ptr<statement>);
}

namespace rush::ast {
	class function_declaration : public nominal_declaration {
		struct factory_tag_t {};

      friend std::unique_ptr<function_declaration> decls::function(
         std::string,
         ast::type_ref,
         std::unique_ptr<pattern>,
         std::unique_ptr<statement>);

	public:
		function_declaration(
			std::string name,
         ast::type_ref return_type,
         std::unique_ptr<ast::pattern> params,
			std::unique_ptr<ast::statement> body,
			factory_tag_t)
         : _name { std::move(name) }
         , _type { types::undefined }
         , _explicit_return_type { std::move(return_type) }
         , _params { decls::parameter(std::move(params)) }
			, _body { std::move(body) } {}

      virtual std::string_view name() const noexcept override {
         return _name;
      }

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::function;
		}

      virtual ast::type_ref type() const noexcept override {
         return resolve_type();
      }

      ast::type_ref return_type() const noexcept {
         return resolve_explicit_return_type();
      }

		ast::pattern const& parameters() const noexcept {
         return _params->pattern();
		}

		ast::statement const& body() const noexcept {
			return *_body;
		}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_function_decl(*this);
      }

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         attach(*_params);
         attach(*_body);
         _type = context()->function_type(*this);
      }

      virtual void detached(ast::node*, ast::context&) override {
         _type = types::undefined;
         detach(*_params);
         detach(*_body);
      }

	private:
      std::string _name;
      mutable std::variant<
         ast::type_ref,
         ast::type_resolver*> _type;
      ast::type_ref _explicit_return_type;
		std::unique_ptr<ast::statement> _body;
      std::unique_ptr<ast::storage_declaration> _params;

      ast::type_ref resolve_type() const {
         struct type_visitor {
            std::variant<ast::type_ref, ast::type_resolver*>& _v;
            ast::type_ref operator ()(ast::type_ref type) { return type; }
            ast::type_ref operator ()(ast::type_resolver* r) { return std::get<ast::type_ref>(_v = r->resolve()); } };
         return std::visit(type_visitor { _type }, _type);
      }

      ast::type_ref resolve_explicit_return_type() const {
         struct return_type_visitor : ast::visitor {
            ast::type_ref result_type = ast::types::undefined;
            virtual void visit_function_type(ast::function_type const& type) override
            { result_type = type.return_type(); } };

         return _explicit_return_type == types::undefined
              ? rush::visit(type(), return_type_visitor {}).result_type
              : _explicit_return_type;
      }
	};

	namespace decls {
		inline std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
               ast::types::undefined,
               ptrns::list(),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type_ref return_type,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
					std::move(return_type),
               ptrns::list(),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<pattern> params,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
               ast::types::undefined,
					std::move(params),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
         ast::type_ref return_type,
			std::unique_ptr<pattern> params,
			std::unique_ptr<statement> body) {
            return std::make_unique<function_declaration>(
               std::move(name),
               std::move(return_type),
               std::move(params),
               std::move(body),
               function_declaration::factory_tag_t {});
			}
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_FUNCTION_HPP
