#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"

namespace rush::ast {
	class type : public node {
	public:
		bool is_known() const noexcept;
		bool is_builtin() const noexcept;

		virtual void accept(ast::visitor&) const;
	};
}

#endif // RUSH_AST_TYPE_HPP
