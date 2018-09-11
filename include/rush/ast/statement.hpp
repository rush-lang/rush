#pragma once

#ifndef RUSH_AST_STATEMENT_HPP
#define RUSH_AST_STATEMENT_HPP

#include "rush/ast/node.hpp"
#include "rush/sema/scope.hpp"

#include <cstdint>

namespace rush::ast {
	enum class statement_kind : std::uint8_t {
		loop,
		branch,
		control,
		expression,
		declaration,
		block,
	};

	class statement : public node {
	public:
		rush::scope& scope() noexcept;
		virtual statement_kind kind() const noexcept = 0;
	private:
		// rush::scope& _scope;
	};
} // rush::ast

#include "rush/ast/stmts/block.hpp"
#include "rush/ast/stmts/control.hpp"
#include "rush/ast/stmts/branch.hpp"
#include "rush/ast/stmts/loop.hpp"

#endif // RUSH_AST_STATEMENT_HPP
