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

		template <typename LiteralExprT>
		friend std::unique_ptr<LiteralExprT> make_literal_expr(
			variant_type val, type const& val_type);

	public:
		literal_expression(variant_type val, ast::type const& val_type, factory_tag_t)
			: _val(std::move(val))
			, _type(val_type) {}

		virtual ast::type result_type() const noexcept override {
			return _type;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_literal_expr(*this);
		}

	protected:
		template <typename T>
		T const& value() const {
			return std::get<T>(_val);
		}

	private:
		variant_type _val;
		type _type;
	};

	class nil_literal_expression : public literal_expression {
	public:
		using literal_expression::literal_expression;
		virtual void accept(ast::visitor& v) const override { v.visit_literal_expr(*this); }
	};

	class string_literal_expression : public literal_expression {
	public:
		using literal_expression::literal_expression;
		std::string value() const { return literal_expression::value<std::string>(); }
		virtual void accept(ast::visitor& v) const override { v.visit_literal_expr(*this); }
	};

	class boolean_literal_expression : public literal_expression {
	public:
		using literal_expression::literal_expression;
		bool value() const { return literal_expression::value<bool>(); }
		virtual void accept(ast::visitor& v) const override { v.visit_literal_expr(*this); }
	};

	class integer_literal_expression : public literal_expression {
	public:
		using literal_expression::literal_expression;
		std::uint64_t value() const { return literal_expression::value<std::uint64_t>(); }
		virtual void accept(ast::visitor& v) const override { v.visit_literal_expr(*this); }
	};

	class floating_literal_expression : public literal_expression {
	public:
		using literal_expression::literal_expression;
		double value() const { return literal_expression::value<double>(); }
		virtual void accept(ast::visitor& v) const override { v.visit_literal_expr(*this); }
	};

	std::unique_ptr<nil_literal_expression> nil_expr(ast::type const&);

	std::unique_ptr<boolean_literal_expression> literal_expr(bool);
	std::unique_ptr<string_literal_expression>  literal_expr(std::string);

	std::unique_ptr<integer_literal_expression> literal_expr(std::int8_t, ast::type const& = sbyte_type);
	std::unique_ptr<integer_literal_expression> literal_expr(std::int16_t, ast::type const& = short_type);
	std::unique_ptr<integer_literal_expression> literal_expr(std::int32_t, ast::type const& = int_type);
	std::unique_ptr<integer_literal_expression> literal_expr(std::int64_t, ast::type const& = long_type);

	std::unique_ptr<integer_literal_expression> literal_expr(std::uint8_t, ast::type const& = byte_type);
	std::unique_ptr<integer_literal_expression> literal_expr(std::uint16_t, ast::type const& = ushort_type);
	std::unique_ptr<integer_literal_expression> literal_expr(std::uint32_t, ast::type const& = uint_type);
	std::unique_ptr<integer_literal_expression> literal_expr(std::uint64_t, ast::type const& = uint_type);

	std::unique_ptr<floating_literal_expression> literal_expr(float, ast::type const& = float_type);
	std::unique_ptr<floating_literal_expression> literal_expr(double, ast::type const& = double_type);
	std::unique_ptr<floating_literal_expression> literal_expr(long double, ast::type const& = double_type);
} // rush

#endif // RUSH_AST_LITERAL_HPP
