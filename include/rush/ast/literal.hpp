#pragma once

#ifndef RUSH_AST_LITERAL_HPP
#define RUSH_AST_LITERAL_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/type.hpp"

#include <variant>
#include <string>

namespace rush::ast {
	class literal_expression : public expression {
		friend std::unique_ptr<literal_expression> literal_expr(std::string);
		friend std::unique_ptr<literal_expression> literal_expr(std::uint64_t, type const&);

		using variant_type = std::variant<
			std::string,
			std::uint64_t,
			double>;
	public:

		virtual void accept(ast::visitor&) const override;
	private:
		variant_type _val;
		literal_expression(variant_type val) : _val(std::move(val)) {}
	};

	std::unique_ptr<literal_expression> literal_expr(std::string);
	std::unique_ptr<literal_expression> literal_expr(std::uint64_t, type const&);
} // rush

#endif // RUSH_AST_LITERAL_HPP
