#pragma once

#ifndef RUSH_AST_STMTS_STATEMENT_HPP
#define RUSH_AST_STMTS_STATEMENT_HPP

#include "rush/ast/node.hpp"

#include <cstdint>

namespace rush::ast {
	enum class statement_kind : std::uint8_t {
		loop,
		branch,
		control,
      if_,
      for_loop,
      while_loop,
		expression,
		declaration,
		block,
	};

	class statement : public node {
	public:
		virtual statement_kind kind() const noexcept = 0;
	};
} // rush::ast

#endif // RUSH_AST_STMTS_STATEMENT_HPP
