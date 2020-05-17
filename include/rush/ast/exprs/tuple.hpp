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

#ifndef RUSH_AST_EXPRS_TUPLE_HPP
#define RUSH_AST_EXPRS_TUPLE_HPP

#include "rush/ast/types/builtin.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/exprs/argument.hpp"

#include <algorithm>

namespace rush::ast {
	class tuple_literal_expression;

	namespace exprs {
		std::unique_ptr<ast::tuple_literal_expression> tuple(
			std::unique_ptr<ast::argument_list> args);
	}

	class tuple_literal_expression : public ast::expression {
		struct factory_tag_t {};

		friend std::unique_ptr<ast::tuple_literal_expression> exprs::tuple(
			std::unique_ptr<ast::argument_list> args);

	public:
		tuple_literal_expression(
			std::unique_ptr<ast::argument_list> args,
			factory_tag_t)
			: _type { ast::types::undefined }
			, _args { std::move(args) } {}

		ast::argument_list const& arguments() const noexcept {
			return *_args;
		}

      virtual ast::type_ref result_type() const override {
			return _type;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_literal_expr(*this);
		}

   protected:
      virtual void attached(ast::node*, ast::context&) override {
			std::for_each(_args->begin(), _args->end(),
				[this](auto& a) { attach(*a); });
         _type = context()->tuple_type(*this);
		}

      virtual void detached(ast::node*, ast::context&) override {
			std::for_each(_args->begin(), _args->end(),
				[this](auto& a) { detach(*a); });
         _type = types::undefined;
		}

	private:
		ast::type_ref _type;
		std::unique_ptr<ast::argument_list> _args;
	};

	namespace exprs {
		inline std::unique_ptr<ast::tuple_literal_expression> tuple(
			std::unique_ptr<ast::argument_list> args) {
				return std::make_unique<ast::tuple_literal_expression>(
					std::move(args),
					tuple_literal_expression::factory_tag_t {});
			}
	}
}

#endif // RUSH_AST_EXPRS_TUPLE_HPP
