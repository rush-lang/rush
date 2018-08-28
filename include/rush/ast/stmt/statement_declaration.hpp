#pragma once

#ifndef RUSH_AST_STMT_STATEMENT_DECLARATION_HPP
#define RUSH_AST_STMT_STATEMENT_DECLARATION_HPP

#include "rush/ast/declaration.hpp"
#include "rush/ast/statement.hpp"

namespace rush::ast {

	// simple wrapper around declarations so that some
	// declarations may be treated as statements
	// (e.g. constants, variables, etc.)
	class statement_declaration : public statement {
	public:
		statement_declaration(std::unique_ptr<declaration> decl)
			: _decl(std::move(decl)) {}

		virtual statement_kind kind() const noexcept override {
			// todo: fix.
			return statement_kind::return_;
		}

		declaration const* const decl() const noexcept {
			return _decl.get();
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			_decl->accept(v);
		}

	private:
		std::unique_ptr<declaration> _decl;
	};

	namespace stmts {
		inline std::unique_ptr<statement_declaration> constant(
			rush::scope& scope,
			std::string name,
			ast::type type,
			std::unique_ptr<expression> init) {
				return std::make_unique<statement_declaration>(
					decls::constant(scope, std::move(name), type, std::move(init)));
			}

		inline std::unique_ptr<statement_declaration> constant(
			rush::scope& scope,
			std::string name,
			std::unique_ptr<expression> init ) {
				return std::make_unique<statement_declaration>(
					decls::constant(scope, std::move(name), std::move(init)));
			}

		inline std::unique_ptr<statement_declaration> constant(
			std::string name,
			ast::type type,
			std::unique_ptr<expression> init) {
				return std::make_unique<statement_declaration>(
					decls::constant(std::move(name), type, std::move(init)));
			}

		inline std::unique_ptr<statement_declaration> constant(
			std::string name,
			std::unique_ptr<expression> init) {
				return std::make_unique<statement_declaration>(
					decls::constant(std::move(name), std::move(init)));
			}


		inline std::unique_ptr<statement_declaration> variable(
			rush::scope& scope,
			std::string name,
			ast::type type,
			std::unique_ptr<expression> init) {
				return std::make_unique<statement_declaration>(
					decls::variable(scope, std::move(name), type, std::move(init)));
			}

		inline std::unique_ptr<statement_declaration> variable(
			rush::scope& scope,
			std::string name,
			std::unique_ptr<expression> init ) {
				return std::make_unique<statement_declaration>(
					decls::variable(scope, std::move(name), std::move(init)));
			}

		inline std::unique_ptr<statement_declaration> variable(
			std::string name,
			ast::type type,
			std::unique_ptr<expression> init) {
				return std::make_unique<statement_declaration>(
					decls::variable(std::move(name), type, std::move(init)));
			}

		inline std::unique_ptr<statement_declaration> variable(
			std::string name,
			std::unique_ptr<expression> init) {
				return std::make_unique<statement_declaration>(
					decls::variable(std::move(name), std::move(init)));
			}
	}
} // rush::ast

#endif // RUSH_AST_STMT_STATEMENT_DECLARATION_HPP
