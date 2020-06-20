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

#ifndef RUSH_AST_STMTS_DECLARATION_HPP
#define RUSH_AST_STMTS_DECLARATION_HPP

#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/stmts/statement.hpp"

#include <type_traits>

namespace rush::ast {

	// simple wrapper around declarations so that some
	// declarations may be treated as statements
	// (e.g. constants, variables, etc.)
	class declaration_statement : public statement {
	public:
		declaration_statement(std::unique_ptr<declaration> decl)
			: _decl(std::move(decl)) {}

      virtual ast::statement_kind kind() const noexcept override {
         return ast::statement_kind::declaration;
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			_decl->accept(v);
		}

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_decl, parent());
      }

      virtual void detached(ast::context&) override {
         detach(*_decl);
      }

	private:
		std::unique_ptr<declaration> _decl;
	};

   namespace stmts {
      inline std::unique_ptr<declaration_statement> decl_stmt(std::unique_ptr<declaration> decl) {
         if (decl == nullptr) throw new std::invalid_argument("decl == nullptr");
         return std::make_unique<declaration_statement>(std::move(decl));
      }
   }
} // rush::ast::details

#endif // RUSH_AST_STMTS_DECLARATION_HPP
