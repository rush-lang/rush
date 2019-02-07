#pragma once

#ifndef RUSH_AST_EXPRS_IDENTIFIER_HPP
#define RUSH_AST_EXPRS_IDENTIFIER_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace rush::ast {
	class identifier_expression;
	namespace exprs { std::unique_ptr<identifier_expression> identifier(std::string name); }
}

namespace rush::ast {
	// \brief Represents an identifier within an expression
	//        such as a variable, or constant.
	class identifier_expression : public expression {
		struct factory_tag_t {};
		friend std::unique_ptr<identifier_expression>
			exprs::identifier(std::string);

	public:
		identifier_expression(std::string name, factory_tag_t) noexcept
			: _name { std::move(name) } {}

		std::string name() const {
			return _name;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_identifier_expr(*this);
		}

	private:
		std::string _name;
	};

	namespace exprs {
		inline std::unique_ptr<identifier_expression> identifier(std::string name) {
			return std::make_unique<identifier_expression>(std::move(name), identifier_expression::factory_tag_t {});
		}
	} // rush::ast::exprs
} // rush::ast

#endif // RUSH_AST_EXPRS_IDENTIFIER_HPP
