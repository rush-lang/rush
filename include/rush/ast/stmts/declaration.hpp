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
			: statement { ast::statement_kind::declaration }
         , _decl(std::move(decl)) {}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			_decl->accept(v);
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
