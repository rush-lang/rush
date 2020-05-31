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

#ifndef RUSH_AST_STMTS_STATEMENT_HPP
#define RUSH_AST_STMTS_STATEMENT_HPP

#include "rush/ast/node.hpp"

#include <cstdint>

namespace rush::ast {
#  define RUSH_STATEMENT_KIND_ENUM
#  include "rush/ast/stmts/_statements.hpp"

	class statement : public virtual node {
	public:
		virtual ast::statement_kind kind() const = 0;
   protected:
      statement() = default;
	};
} // rush::ast

#endif // RUSH_AST_STMTS_STATEMENT_HPP
