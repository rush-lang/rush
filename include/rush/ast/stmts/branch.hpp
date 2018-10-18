#pragma once

#ifndef RUSH_AST_STMTS_BRANCH_HPP
#define RUSH_AST_STMTS_BRANCH_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/stmts/statement.hpp"

namespace rush::ast {
	class if_statement {
	private:
		std::unique_ptr<expression> _cond;
		std::unique_ptr<statement> _then;
		std::unique_ptr<statement> _else;
	};

	class case_statement {
	private:

	};

	class switch_statement {
	private:
		std::unique_ptr<expression> _expr;
		std::vector<std::vector<case_statement>> _cases;
	};

	namespace stmts {
		std::unique_ptr<if_statement> if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then);

		std::unique_ptr<if_statement> if_(
			std::unique_ptr<expression> cond,
			std::unique_ptr<statement> then,
			std::unique_ptr<statement> else_);


		std::unique_ptr<case_statement> case_(
			// some pattern type,
			std::unique_ptr<statement> body);

		std::unique_ptr<switch_statement> switch_(
			std::unique_ptr<expression> expr,
			std::initializer_list<std::unique_ptr<case_statement>> cases);
	}
}

#endif // RUSH_AST_STMTSS_BRANCH_HPP
