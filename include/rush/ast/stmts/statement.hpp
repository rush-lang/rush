#pragma once

#ifndef RUSH_AST_STMTS_STATEMENT_HPP
#define RUSH_AST_STMTS_STATEMENT_HPP

#include "rush/ast/node.hpp"

#include <cstdint>

namespace rush::ast {
#  define RUSH_STATEMENT_KIND_ENUM
#  include "rush/ast/stmts/_statements.hpp"

	class statement : public node {
	public:
		statement_kind kind() const noexcept {
         return _kind;
      };

   protected:
      statement(statement_kind kind)
         : _kind { kind } {}

   private:
      statement_kind _kind;
	};
} // rush::ast

#endif // RUSH_AST_STMTS_STATEMENT_HPP
