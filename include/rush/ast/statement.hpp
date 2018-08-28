#pragma once

#ifndef RUSH_AST_STATEMENT_HPP
#define RUSH_AST_STATEMENT_HPP

#include "rush/ast/node.hpp"
#include "rush/sema/scope.hpp"

namespace rush::ast {
	enum class statement_kind {
		block,
		return_,
	};

	class statement : public node {
	public:
		virtual statement_kind kind() const noexcept = 0;
		rush::scope& scope() noexcept;
	private:
		// rush::scope& _scope;
	};
} // rush::ast

#include "rush/ast/stmt/statement_block.hpp"
#include "rush/ast/stmt/statement_declaration.hpp"
#include "rush/ast/stmt/return.hpp"

#endif // RUSH_AST_STATEMENT_HPP
