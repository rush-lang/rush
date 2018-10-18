#pragma once

#ifndef RUSH_AST_ARGUMENT_HPP
#define RUSH_AST_ARGUMENT_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/sema/scope.hpp"
#include "rush/sema/symbol.hpp"

#include <vector>

namespace rush::ast {
	class argument : public node {
	public:

	private:
		std::unique_ptr<expression> _expr;
	};

	class named_argument : public argument {
	public:
	private:
		sema::symbol _ident;
	};

	namespace exprs {
		std::unique_ptr<argument> arg(std::unique_ptr<expression> expr);
		std::unique_ptr<named_argument> arg(std::string name, std::unique_ptr<expression> expr);
	}
} // rush::ast

#endif // RUSH_AST_ARGUMENT_HPP
