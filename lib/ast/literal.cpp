#include "rush/ast/literal.hpp"

namespace rush::ast {
	inline std::unique_ptr<literal_expression> make_literal_expr(
		literal_expression::variant_type val,
		ast::type const& val_type
	) {
		return std::make_unique<literal_expression>(
			std::move(val), val_type,
			literal_expression::factory_tag_t{}
		);
	}

	std::unique_ptr<literal_expression> literal_expr(bool val) {
		return make_literal_expr(val, ast::bool_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::string val) {
		return make_literal_expr(val, ast::string_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::int8_t val, ast::type const& val_type) {
		return make_literal_expr(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::int16_t val, ast::type const& val_type) {
		return make_literal_expr(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::int32_t val, ast::type const& val_type) {
		return make_literal_expr(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::int64_t val, ast::type const& val_type) {
		return make_literal_expr(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::uint8_t val, ast::type const& val_type) {
		return make_literal_expr(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::uint16_t val, ast::type const& val_type) {
		return make_literal_expr(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::uint32_t val, ast::type const& val_type) {
		return make_literal_expr(static_cast<std::uint64_t>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(std::uint64_t val, ast::type const& val_type) {
		return make_literal_expr(val, val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(float val, ast::type const& val_type) {
		return make_literal_expr(static_cast<double>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(double val, ast::type const& val_type) {
		return make_literal_expr(static_cast<double>(val), val_type);
	}

	std::unique_ptr<literal_expression> literal_expr(long double val, ast::type const& val_type) {
		return make_literal_expr(static_cast<double>(val), val_type);
	}
}