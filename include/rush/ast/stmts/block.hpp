#pragma once

#ifndef RUSH_AST_STMT_BLOCK_STATEMENT_HPP
#define RUSH_AST_STMT_BLOCK_STATEMENT_HPP

#include "rush/ast/statement.hpp"

#include <vector>

namespace rush::ast {
	class statement_block : public statement {
	public:
		virtual statement_kind kind() const noexcept override {
			return statement_kind::block;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {

		}

	private:
		std::vector<std::unique_ptr<statement>> _stmts;
	};

	namespace stmts {
		inline std::unique_ptr<statement_block> block(
			std::initializer_list<std::unique_ptr<statement>> init) {
			return nullptr;
		}
	} // stmts
} // rush::ast

#endif // RUSH_AST_STMT_BLOCK_STATEMENT_HPP
