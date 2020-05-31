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

#ifndef RUSH_AST_STMT_STATEMENT_BLOCK_HPP
#define RUSH_AST_STMT_STATEMENT_BLOCK_HPP

#include "rush/extra/dereferencing_iterator.hpp"

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/stmts/forwarding.hpp"

#include <vector>

namespace rush::ast {
	class statement_block : public ast::node_list<ast::statement> {
	public:
      using node_list<ast::statement>::accept;
      using node_list<ast::statement>::node_list;

      virtual ast::statement_kind kind() const noexcept override {
         return ast::statement_kind::block;
      }

		virtual void accept(ast::visitor& v) const override {
			v.visit_block_stmt(*this);
		}
	};

	namespace stmts {
      inline std::unique_ptr<statement_block> block() {
         return std::make_unique<ast::statement_block>();
      }

      inline std::unique_ptr<statement_block> block(std::vector<std::unique_ptr<ast::statement>> stmts) {
         return std::make_unique<statement_block>(std::move(stmts));
      }
	} // rush::ast::stmts
} // rush::ast

#endif // RUSH_AST_STMT_STATEMENT_BLOCK_HPP
