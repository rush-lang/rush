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

#ifndef RUSH_AST_EXPRS_IDENTIFIER_HPP
#define RUSH_AST_EXPRS_IDENTIFIER_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/nominal.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/scope.hpp"

#include <string>
#include <vector>
#include <cassert>

namespace rush::ast {
	class identifier_expression;
	namespace exprs {
      std::unique_ptr<identifier_expression> identifier(std::string name);
   }
}

namespace rush::ast {
   class nominal_declaration;

	// \brief Represents the identifier of a variable, constant
   //        or function within an expression.
	class identifier_expression : public expression {
		struct factory_tag_t {};
		friend std::unique_ptr<identifier_expression> exprs::identifier(std::string name);

	public:
		identifier_expression(std::string name, factory_tag_t) noexcept
			: _name { std::move(name) } {}

      bool is_unresolved() const noexcept {
         return _decl->kind() == ast::declaration_kind::undeclared;
      }

      bool is_overloaded() const noexcept {
         return _decl->kind() == ast::declaration_kind::overloaded;
      }

		std::string_view name() const {
         return _name;
		}

      virtual ast::type_ref result_type() const noexcept override {
         return _decl->type();
      };

      ast::nominal_declaration const& declaration() const noexcept {
         return *_decl;
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_identifier_expr(*this);
		}

   protected:
      virtual void attached(ast::scope& scope, ast::context& context) override;
      virtual void detached(ast::context&) override;

	private:
      std::string _name;
      ast::nominal_declaration const* _decl;
	};

	namespace exprs {
		inline std::unique_ptr<identifier_expression> identifier(std::string name) {
			return std::make_unique<identifier_expression>(
            std::move(name),
            identifier_expression::factory_tag_t {});
		}
	} // rush::ast::exprs
} // rush::ast

#endif // RUSH_AST_EXPRS_IDENTIFIER_HPP
