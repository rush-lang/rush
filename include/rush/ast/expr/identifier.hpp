#pragma once

#ifndef RUSH_AST_IDENTIFIER_HPP
#define RUSH_AST_IDENTIFIER_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace rush::ast {
	// \brief Represents an identifier within an expression
	//        such as a variable, or constant.
	class identifier_expression : public expression {
		struct factory_tag_t {};
		friend std::unique_ptr<identifier_expression>
			identifier_expr(std::string name);

	public:
		identifier_expression(std::string name, factory_tag_t) noexcept
			: _name(std::move(name)) {}

		std::string name() const {
			return _name;
		}

		virtual void accept(ast::visitor& vis) const override {
			vis.visit_identifier_expr(*this);
		}

	private:
		std::string _name;
	};

	inline std::unique_ptr<identifier_expression> identifier_expr(std::string name) {
		return std::make_unique<identifier_expression>(name, identifier_expression::factory_tag_t{});
	}
} // rush

#endif // RUSH_AST_IDENTIFIER_HPP
