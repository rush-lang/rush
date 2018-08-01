#pragma once

#ifndef RUSH_AST_IDENTIFIER_HPP
#define RUSH_AST_IDENTIFIER_HPP

#include "rush/ast/expr/expression.hpp"

#include <string>

namespace rush::ast {
	// \brief Represents an identifier within an expression
	//        such as a variable, or constant.
	class identifier_expression : public expression {
	public:
		std::string name() const {
			return _name;
		}

		virtual void accept(ast::visitor&) const override;
	private:
		identifier_expression(std::string name) noexcept(
			std::is_nothrow_move_constructible_v<std::string>)
			: _name(std::move(name)) {}

		std::string _name;

		friend identifier_expression identifier_expr(std::string name);
	};

	inline identifier_expression identifier_expr(std::string name) {
		return { name };
	}
} // rush

#endif // RUSH_AST_IDENTIFIER_HPP
