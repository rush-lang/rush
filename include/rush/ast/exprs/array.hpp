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
#include "rush/ast/context.hpp"

#include <algorithm>

namespace rush::ast {
	class array_literal_expression;
	using element_list = detail::basic_list<ast::expression>;

	namespace exprs {
		std::unique_ptr<ast::array_literal_expression> array(
			std::unique_ptr<element_list> elems);
	}

	class array_literal_expression : public ast::expression {
		struct factory_tag_t {};

		friend std::unique_ptr<ast::array_literal_expression> exprs::array(
			std::unique_ptr<element_list> elems);

	public:
		array_literal_expression(
			std::unique_ptr<element_list> elems,
			factory_tag_t)
			: _type { ast::types::undefined }
			, _elems { std::move(elems) } {}

		element_list const& elements() const noexcept {
			return *_elems;
		}

      virtual ast::type_ref result_type() const override {
			return _type;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_literal_expr(*this);
		}

      virtual void attach(ast::node&, ast::context& context) override {
			std::for_each(_elems->begin(), _elems->end(),
				[this, &context](auto& a) { a->attach(*this, context); });
         _type = context.array_type(*this);
		}

      virtual void detach(ast::node&, ast::context& context) override {
			std::for_each(_elems->begin(), _elems->end(),
				[this, &context](auto& a) { a->detach(*this, context); });
			_type = ast::types::undefined;
		}

	private:
		ast::type_ref _type;
		std::unique_ptr<element_list> _elems;
	};

	namespace exprs {
		inline std::unique_ptr<ast::array_literal_expression> array(
			std::unique_ptr<element_list> elems) {
				return std::make_unique<ast::array_literal_expression>(
					std::move(elems),
					array_literal_expression::factory_tag_t {});
			}

      inline std::unique_ptr<ast::element_list> elem_list() {
         return std::make_unique<ast::element_list>();
      }

      inline std::unique_ptr<ast::element_list> elem_list(std::vector<std::unique_ptr<ast::expression>> args) {
         return std::make_unique<ast::element_list>(std::move(args));
      }
	}
}

#endif // RUSH_AST_EXPRS_ARRAY_HPP
