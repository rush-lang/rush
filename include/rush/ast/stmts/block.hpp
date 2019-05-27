#pragma once

#ifndef RUSH_AST_STMT_STATEMENT_BLOCK_HPP
#define RUSH_AST_STMT_STATEMENT_BLOCK_HPP

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
			: _stmts(std::move(stmts)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::block;
		}

      const_iterator begin() const noexcept { return _stmts.begin(); }
      const_iterator end() const noexcept { return _stmts.end(); }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_block_stmt(*this);
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
