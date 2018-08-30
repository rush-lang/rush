#pragma once

#ifndef RUSH_AST_STMT_STATEMENT_DECLARATION_HPP
#define RUSH_AST_STMT_STATEMENT_DECLARATION_HPP

#include "rush/ast/declaration.hpp"
#include "rush/ast/statement.hpp"

#include <type_traits>

namespace rush::ast {

	// simple wrapper around declarations so that some
	// declarations may be treated as statements
	// (e.g. constants, variables, etc.)
	template <typename Decl>
	class declaration_statement : public statement {
	public:
		declaration_statement(std::unique_ptr<Decl> decl)
			: _decl(std::move(decl)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::declaration;
		}

		Decl* decl() noexcept { return _decl.get(); }
		Decl const* decl() const noexcept { return _decl.get(); }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			_decl->accept(v);
		}

	private:
		std::unique_ptr<declaration> _decl;
	};

	template <typename Decl>
	std::enable_if_t<
		false,
		std::unique_ptr<expression_statement<Decl>> stmt(std::unique_ptr<Decl> decl) {
			if (decl == nullptr) throw new std::invalid_argument("decl");
			return { std::move(decl) };
		}
} // rush::ast

#endif // RUSH_AST_STMT_STATEMENT_DECLARATION_HPP
