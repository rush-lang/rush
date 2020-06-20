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

#ifndef RUSH_AST_TYPES_TUPLE_HPP
#define RUSH_AST_TYPES_TUPLE_HPP

#include "rush/extra/iterator_range.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/type_ref.hpp"

#include <vector>

namespace rush::ast {
	class tuple_type;

	template <>
	struct type_traits<tuple_type> {
		static constexpr ast::type_kind kind() noexcept {
			return ast::type_kind::tuple;
		}
	};

	class tuple_type : public type {
	public:
		explicit tuple_type(std::vector<ast::type_ref> types)
			: _types { std::move(types) } {}

		std::vector<ast::type_ref> const& types() const noexcept {
			return _types;
		}

      virtual ast::type_kind kind() const override {
			return type_traits<tuple_type>::kind();
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_tuple_type(*this);
		}

   protected:
      virtual void attached(ast::scope&, ast::context&) override {}
      virtual void detached(ast::context&) override {}

	private:
		std::vector<ast::type_ref> _types;
	};
}

#endif // RUSH_AST_TYPES_TUPLE_HPP
