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
#include "rush/ast/exprs/binary.hpp"

namespace rush::ast {
	inline std::unique_ptr<binary_expression> make_binary_expr(
		std::unique_ptr<expression> left,
		std::unique_ptr<expression> right,
		binary_operator op
	) {
		return std::make_unique<binary_expression>(
			std::move(left),
			std::move(right),
			op, binary_expression::factory_tag_t{}
		);
	}
} // rush::ast

#define RUSH_BINARY_EXPRESSION_FACTORY_FUNC_DEFNS
#include "rush/ast/exprs/_expressions.hpp"