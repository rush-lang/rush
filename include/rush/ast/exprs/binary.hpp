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

#ifndef RUSH_AST_EXPRS_BINARY_HPP
#define RUSH_AST_EXPRS_BINARY_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/types/type_reducer.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/context.hpp"

#include <memory>

#define RUSH_BINARY_OPERATORS_ENUM
#include "rush/ast/exprs/_operators.hpp"

namespace rush::ast {
	class binary_expression : public expression {
		struct factory_tag_t {};

	public:
		binary_expression(
			std::unique_ptr<expression> left,
			std::unique_ptr<expression> right,
			binary_operator op, factory_tag_t)
			: _left(std::move(left))
			, _right(std::move(right))
			, _opkind(op) {}

		binary_operator opkind() const noexcept { return _opkind; }
		expression const& left_operand() const noexcept { return *_left; }
		expression const& right_operand() const noexcept { return *_right; }

      virtual ast::type_ref result_type() const noexcept override {
         switch (_opkind) {
         case binary_operator::equal:
         case binary_operator::not_equal:
         case binary_operator::logical_or:
         case binary_operator::logical_and:
         case binary_operator::less_than:
         case binary_operator::less_equals:
         case binary_operator::greater_than:
         case binary_operator::greater_equals:
            return context()->bool_type();
         default: return types::reduce(
            _left->result_type(),
            _right->result_type());
         }
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_binary_expr(*this);
		}

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         attach(*_left);
         attach(*_right);
      }

      virtual void detached(ast::node*, ast::context&) override {
         detach(*_left);
         detach(*_right);
      }

	private:
		std::unique_ptr<expression> _left;
		std::unique_ptr<expression> _right;
		binary_operator _opkind;

		friend std::unique_ptr<binary_expression> make_binary_expr(
			std::unique_ptr<expression> left,
			std::unique_ptr<expression> right,
			binary_operator op
		);
	};
} // rush

#define RUSH_BINARY_EXPRESSION_FACTORY_FUNC_DECLS
#include "rush/ast/exprs/_expressions.hpp"

#endif // RUSH_AST_EXPRS_BINARY_HPP
