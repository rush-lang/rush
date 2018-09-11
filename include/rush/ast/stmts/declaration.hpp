#pragma once

#ifndef RUSH_AST_STMTS_DECLARATION_HPP
#define RUSH_AST_STMTS_DECLARATION_HPP

#include "rush/ast/declaration.hpp"
#include "rush/ast/statement.hpp"

#include <type_traits>

namespace rush::ast::details {

	// simple wrapper around declarations so that some
	// declarations may be treated as statements
	// (e.g. constants, variables, etc.)
	class declaration_statement : public statement {
	public:
		declaration_statement(std::unique_ptr<declaration> decl)
			: _decl(std::move(decl)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::declaration;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			_decl->accept(v);
		}

	private:
		std::unique_ptr<declaration> _decl;
	};

	inline std::unique_ptr<declaration_statement> decl_stmt(std::unique_ptr<declaration> decl) {
		if (decl == nullptr) throw new std::invalid_argument("decl == nullptr");
		return std::make_unique<declaration_statement>(std::move(decl));
	}
} // rush::ast::details

#endif // RUSH_AST_STMTS_DECLARATION_HPP
