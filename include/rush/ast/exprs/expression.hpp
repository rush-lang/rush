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

#ifndef RUSH_AST_EXPRS_EXPRESSION_HPP
#define RUSH_AST_EXPRS_EXPRESSION_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types/type_ref.hpp"

#include <utility>
#include <memory>

namespace rush::ast {

	// Expressions are a tree-like structure with a possibly centralized pool of
	// memory, enabled via move semantics (?? possibly ??)
	class expression : public virtual node {
	protected:
		expression(expression const&) = delete;
		void operator = (expression const&) = delete;

	public:
		expression() = default;

      virtual ast::type_ref result_type() const = 0;
	};
}

#endif // RUSH_AST_EXPRS_EXPRESSION_HPP
