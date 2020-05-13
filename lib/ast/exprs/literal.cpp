/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#include "rush/ast/context.hpp"
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/types/builtin.hpp"

namespace rush::ast {
	template <typename LiteralExprT>
	inline std::unique_ptr<LiteralExprT> make_literal_expr(
		literal_expression::variant_type val,
		ast::type_ref val_type
	) {
		return std::make_unique<LiteralExprT>(
			std::move(val), val_type,
			literal_expression::factory_tag_t{}
		);
	}
}

namespace rush::ast::exprs {
	std::unique_ptr<nil_literal_expression> nil(ast::type_ref type) {
		return make_literal_expr<nil_literal_expression>(std::uint64_t(0), type);
	}

	std::unique_ptr<boolean_literal_expression> literal(bool val, ast::context& ctx) {
		return make_literal_expr<boolean_literal_expression>(val, ctx.bool_type());
	}

	std::unique_ptr<string_literal_expression> literal(std::string val, ast::context& ctx) {
		return make_literal_expr<string_literal_expression>(val, ctx.string_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::int8_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), ctx.int8_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::int16_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), ctx.int16_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::int32_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), ctx.int32_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::int64_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), ctx.int64_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint8_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), ctx.uint8_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint16_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), ctx.uint16_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint32_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(static_cast<std::uint64_t>(val), ctx.uint32_type());
	}

	std::unique_ptr<integer_literal_expression> literal(std::uint64_t val, ast::context& ctx) {
		return make_literal_expr<integer_literal_expression>(val, ctx.uint64_type());
	}

	std::unique_ptr<floating_literal_expression> literal(float val, ast::context& ctx) {
		return make_literal_expr<floating_literal_expression>(static_cast<double>(val), ctx.ieee32_type());
	}

	std::unique_ptr<floating_literal_expression> literal(double val, ast::context& ctx) {
		return make_literal_expr<floating_literal_expression>(static_cast<double>(val), ctx.ieee64_type());
	}

	std::unique_ptr<floating_literal_expression> literal(long double val, ast::context& ctx) {
		return make_literal_expr<floating_literal_expression>(static_cast<double>(val), ctx.ieee64_type());
	}
}
