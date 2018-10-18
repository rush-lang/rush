#pragma once

#ifndef RUSH_AST_STMT_BLOCK_STATEMENT_HPP
#define RUSH_AST_STMT_BLOCK_STATEMENT_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/stmts/forwarding.hpp"

#include <vector>

namespace rush::ast {
	class statement_block : public statement {
	public:
		statement_block(rush::scope scope, std::vector<std::unique_ptr<statement>> stmts)
			: _scope(std::move(scope))
			, _stmts(std::move(stmts)) {}

		virtual statement_kind kind() const noexcept override {
			return statement_kind::block;
		}

		void detach();
		void attach(rush::scope&);

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			// v.visit_block(*this)
		}

	private:
		rush::scope _scope;
		std::vector<std::unique_ptr<statement>> _stmts;
	};

	namespace stmts {
		inline std::unique_ptr<statement_block> block(
			std::initializer_list<details::statement_fwdr> init) {

			// auto scp = rush::scope { };
			// auto vis = detail::attaching_visitor { scp };
			// auto stmts = std::vector<std::unique_ptr<statement>> { init.size() };

			// std::for_each(
			// 	std::make_move_iterator(init.begin()),
			// 	std::make_move_iterator(init.end()),
			// 	[&](std::unique_ptr<statement> stmt) {
			// 		stmt->accept(vis);
			// 		stmts.push_back(std::move(stmt));
			// 	});

			// return { std::move(scp), std::move(stmts) };
			return nullptr;
		}
	} // stmts
} // rush::ast

#endif // RUSH_AST_STMT_BLOCK_STATEMENT_HPP
