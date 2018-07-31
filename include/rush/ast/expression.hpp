#pragma once

#ifndef RUSH_AST_EXPRESSION_HPP
#define RUSH_AST_EXPRESSION_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/type.hpp"

#include <utility>
#include <memory>

namespace rush::ast {

	// Expressions are a tree-like structure with a possibly centralized pool of
	// memory, enabled via move semantics (?? possibly ??)
	class expression : public node {
		ast::type const& result_type() const noexcept;
	};
}

#endif // RUSH_AST_EXPRESSION_HPP
