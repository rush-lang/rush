#pragma once

#ifndef RUSH_AST_STMT_STATEMENT_BLOCK_HPP
#define RUSH_AST_STMT_STATEMENT_BLOCK_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/stmts/forwarding.hpp"

#include <vector>

namespace rush::ast {
	class statement_block;
	namespace stmts { std::unique_ptr<statement_block> block(std::initializer_list<details::statement_fwdr> init); }
}

namespace rush::ast {
	class statement_block : public statement {
		friend std::unique_ptr<statement_block> stmts::block(std::initializer_list<details::statement_fwdr> init);
	public:
		statement_block(std::vector<std::unique_ptr<statement>> stmts)
			: _stmts(std::move(stmts)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::block;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_block_stmt(*this);
		}

	private:
		std::vector<std::unique_ptr<statement>> _stmts;
	};

	namespace stmts {
		inline std::unique_ptr<statement_block> block(std::vector<details::statement_fwdr> stmts) {
			return nullptr;
			// return std::make_unique<statement_block>(std::vector<std::unique_ptr<statement>> { std::move(init) });
		}
	} // rush::ast::stmts
} // rush::ast

#endif // RUSH_AST_STMT_STATEMENT_BLOCK_HPP
