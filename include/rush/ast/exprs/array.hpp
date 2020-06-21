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

#ifndef RUSH_AST_EXPRS_ARRAY_HPP
#define RUSH_AST_EXPRS_ARRAY_HPP

#include "rush/ast/types/builtin.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/exprs/list.hpp"
#include "rush/ast/context.hpp"

#include <algorithm>

namespace rush::ast {
	class array_literal_expression;

	namespace exprs {
		std::unique_ptr<ast::array_literal_expression> array(
			std::unique_ptr<expression_list> elems);
	}

	class array_literal_expression : public ast::expression {
		struct factory_tag_t {};

		friend std::unique_ptr<ast::array_literal_expression> exprs::array(
			std::unique_ptr<ast::expression_list> elems);

	public:
		array_literal_expression(
         ast::type_ref type,
			std::unique_ptr<ast::expression_list> elems,
			factory_tag_t)
			: _type { type }
			, _elems { std::move(elems) }
         { adopt(*_elems); }

		ast::expression_list const& elements() const noexcept {
			return *_elems;
		}

      virtual ast::type_ref result_type() const override {
			return _type == ast::types::undefined
              ? _type = resolve_type()
              : _type;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_literal_expr(*this);
		}

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
			std::for_each(_elems->begin(), _elems->end(),
				[this, &scope](auto& a) { attach(scope, a); });
         _type = context()->array_type(*this);
		}

      virtual void detached(ast::context&) override {
			std::for_each(_elems->begin(), _elems->end(),
				[this](auto& a) { detach(a); });
			_type = ast::types::undefined;
		}

	private:
		mutable ast::type_ref _type;
		std::unique_ptr<ast::expression_list> _elems;

      ast::type_ref resolve_type() const;
	};

	namespace exprs {
		inline std::unique_ptr<ast::array_literal_expression> array(
			std::unique_ptr<ast::expression_list> elems) {
				return std::make_unique<ast::array_literal_expression>(
               ast::types::undefined,
					std::move(elems),
					array_literal_expression::factory_tag_t {});
			}
	}
}

#endif // RUSH_AST_EXPRS_ARRAY_HPP
