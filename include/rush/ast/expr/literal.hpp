#pragma once

#ifndef RUSH_AST_LITERAL_HPP
#define RUSH_AST_LITERAL_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/type.hpp"

#include <variant>
#include <string>

namespace rush::ast {
	class literal_expression : public expression {
		struct factory_tag_t {};

		using variant_type = std::variant<
			std::string,
			std::uint64_t,
			double, bool>;

		friend std::unique_ptr<literal_expression> make_literal_expr(
			variant_type val, type const& val_type);

	public:
		literal_expression(variant_type val, ast::type const& val_type, factory_tag_t)
			: _val(std::move(val))
			, _type(val_type) {}

		virtual type result_type() const noexcept override {
			return _type;
		}

		virtual void accept(ast::visitor& vis) const override {
			vis.visit_literal_expr(*this);
		}

	private:
		variant_type _val;
		type _type;
	};

	std::unique_ptr<literal_expression> literal_expr(bool);
	std::unique_ptr<literal_expression> literal_expr(std::string);

	std::unique_ptr<literal_expression> literal_expr(std::int8_t, ast::type const& = sbyte_type);
	std::unique_ptr<literal_expression> literal_expr(std::int16_t, ast::type const& = short_type);
	std::unique_ptr<literal_expression> literal_expr(std::int32_t, ast::type const& = int_type);
	std::unique_ptr<literal_expression> literal_expr(std::int64_t, ast::type const& = long_type);

	std::unique_ptr<literal_expression> literal_expr(std::uint8_t, ast::type const& = byte_type);
	std::unique_ptr<literal_expression> literal_expr(std::uint16_t, ast::type const& = ushort_type);
	std::unique_ptr<literal_expression> literal_expr(std::uint32_t, ast::type const& = uint_type);
	std::unique_ptr<literal_expression> literal_expr(std::uint64_t, ast::type const& = uint_type);

	std::unique_ptr<literal_expression> literal_expr(float, ast::type const& = float_type);
	std::unique_ptr<literal_expression> literal_expr(double, ast::type const& = double_type);
	std::unique_ptr<literal_expression> literal_expr(long double, ast::type const& = double_type);
} // rush

#endif // RUSH_AST_LITERAL_HPP
