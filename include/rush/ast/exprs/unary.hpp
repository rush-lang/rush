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

#ifndef RUSH_AST_EXPRS_UNARY_HPP
#define RUSH_AST_EXPRS_UNARY_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>

#define RUSH_UNARY_OPERATOR_ENUM
#include "rush/ast/exprs/_operators.hpp"

namespace rush::ast {
	class unary_expression : public expression {
		struct factory_tag_t {};

	public:
		unary_expression(
			std::unique_ptr<expression> operand,
			unary_operator opkind, factory_tag_t)
			: _operand(std::move(operand))
			, _opkind(opkind)
         { adopt(*_operand); }

		unary_operator opkind() const noexcept { return _opkind; }
		expression const& operand() const noexcept { return *_operand; }

      virtual ast::type_ref result_type() const noexcept override {
         return _operand->result_type();
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_unary_expr(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_operand);
      }

      virtual void detached(ast::context&) override {
         detach(*_operand);
      }

	private:
		std::unique_ptr<expression> _operand;
		unary_operator _opkind;

		friend std::unique_ptr<unary_expression> make_unary_expression(
			std::unique_ptr<expression>,
			unary_operator);
	};
} // rush

#define RUSH_UNARY_EXPRESSION_FACTORY_FUNC_DECLS
#include "rush/ast/exprs/_expressions.hpp"

#endif // RUSH_AST_EXPRS_UNARY_HPP
