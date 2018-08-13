#include "rush/ast/expr/literal.hpp"

namespace rush::ast {
	template <typename LiteralExprT>
	inline std::unique_ptr<LiteralExprT> make_literal_expr(
		literal_expression::variant_type val,
		ast::type const& val_type
	) {
		return std::make_unique<LiteralExprT>(
			std::move(val), val_type,
			literal_expression::factory_tag_t{}
		);
	}
}

namespace rush::ast::exprs {
	std::unique_ptr<nil_literal_expression> nil(ast::type const& type) {
		return make_literal_expr<nil_literal_expression>(std::uint64_t(0), type);
	}

	std::unique_ptr<boolean_literal_expression> literal(bool val) {
		return make_literal_expr<boolean_literal_expression>(val, ast::bool_type);
	}

	std::unique_ptr<string_literal_expression> literal(std::string val) {
		return make_literal_expr<string_literal_expression>(val, ast::string_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::int8_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::int16_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::int32_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::int64_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint8_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint16_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint32_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint64_t val, ast::type const& val_type) {
		return make_literal_expr<integer_literal_expression>(val, val_type);
	}

	std::unique_ptr<floating_literal_expression> literal(float val, ast::type const& val_type) {
		return make_literal_expr<floating_literal_expression>(static_cast<double>(val), val_type);
	}

	std::unique_ptr<floating_literal_expression> literal(double val, ast::type const& val_type) {
		return make_literal_expr<floating_literal_expression>(static_cast<double>(val), val_type);
	}

	std::unique_ptr<floating_literal_expression> literal(long double val, ast::type const& val_type) {
		return make_literal_expr<floating_literal_expression>(static_cast<double>(val), val_type);
	}
}