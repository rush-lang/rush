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
	class statement_block;
	namespace stmts {
      std::unique_ptr<statement_block> block(std::vector<std::unique_ptr<ast::statement>> stmts);
   }
}

namespace rush::ast {
	class statement_block : public statement {
      friend std::unique_ptr<statement_block> stmts::block(std::vector<std::unique_ptr<ast::statement>> stmts);
	public:
      using container_type = std::vector<std::unique_ptr<ast::statement>>;

      using const_iterator = typename container_type::const_iterator;

		statement_block(std::vector<std::unique_ptr<ast::statement>> stmts)
			: statement { ast::statement_kind::block }
         , _stmts(std::move(stmts)) {}

      auto empty() const noexcept {
         return _stmts.empty();
      }

      auto size() const noexcept {
         return _stmts.size();
      }

      auto& front() const noexcept {
         return *_stmts.front();
      }

      auto& back() const noexcept {
         return *_stmts.back();
      }

      auto begin() const noexcept {
         return rush::make_deref_iterator(_stmts.begin());
      }

      auto end() const noexcept {
          return rush::make_deref_iterator(_stmts.end());
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_block_stmt(*this);
		}

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         std::for_each(_stmts.begin(), _stmts.end(),
            [this](auto& stmt) { attach(*stmt); });
      }

      virtual void detached(ast::node*, ast::context&) override {
         std::for_each(_stmts.begin(), _stmts.end(),
            [this](auto& stmt) { detach(*stmt); });
      }

	private:
		std::vector<std::unique_ptr<statement>> _stmts;
	};

	namespace stmts {
      inline std::unique_ptr<statement_block> block(std::vector<std::unique_ptr<ast::statement>> stmts) {
         return std::make_unique<statement_block>(std::move(stmts));
      }
	} // rush::ast::stmts
} // rush::ast

#endif // RUSH_AST_STMT_STATEMENT_BLOCK_HPP
