#pragma once

#ifndef RUSH_AST_EXPRS_EXPRESSION_HPP
#define RUSH_AST_EXPRS_EXPRESSION_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/type.hpp"

#include <utility>
#include <memory>

namespace rush::ast {

	// Expressions are a tree-like structure with a possibly centralized pool of
	// memory, enabled via move semantics (?? possibly ??)
	class expression : public node {
	protected:
		expression(expression const&) = delete;
		void operator = (expression const&) = delete;

	public:
		expression() = default;
	};
}

#endif // RUSH_AST_EXPRS_EXPRESSION_HPP
