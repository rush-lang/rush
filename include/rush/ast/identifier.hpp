#pragma once

#ifndef RUSH_AST_IDENTIFIER_HPP
#define RUSH_AST_IDENTIFIER_HPP

#include "rush/ast/expression.hpp"

namespace rush::ast {
	class identifier_expression : public expression {
	public:
		virtual void accept(ast::visitor&) const override;
	};
} // rush

#endif // RUSH_AST_IDENTIFIER_HPP
