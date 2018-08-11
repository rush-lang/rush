#pragma once

#ifndef RUSH_AST_DECLARATION_HPP
#define RUSH_AST_DECLARATION_HPP

#include "rush/ast/node.hpp"

namespace rush::ast {
	class declaration : public node {
	private:
		declaration(declaration const&) = delete;
		void operator = (declaration const&) = delete;

	public:
		declaration() = default;
	};
}

#include "rush/ast/decl/storage.hpp"
#include "rush/ast/decl/function.hpp"

#endif // RUSH_AST_DECLARATION_HPP
