#pragma once

#ifndef RUSH_AST_EXPRS_IDENTIFIER_HPP
#define RUSH_AST_EXPRS_IDENTIFIER_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/sema/symbol.hpp"

#include <string>

namespace rush::ast {
	class identifier_expression;
	namespace exprs { std::unique_ptr<identifier_expression> identifier(scope& scope, std::string name); }
}

namespace rush::ast {
	// \brief Represents an identifier within an expression
	//        such as a variable, or constant.
	class identifier_expression : public expression {
		struct factory_tag_t {};
		friend std::unique_ptr<identifier_expression>
			exprs::identifier(scope&, std::string);

	public:
		identifier_expression(sema::symbol symbol, factory_tag_t) noexcept
			: _symbol(symbol) {}

		std::string name() const {
			return _symbol.name();
		}

		bool is_undefined() const noexcept {
			return _symbol.is_undefined();
		}

		virtual ast::type result_type() const override {
			return { _symbol.type() };
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_identifier_expr(*this);
		}

	private:
		sema::symbol _symbol;
	};
} // rush::ast

namespace rush::ast::exprs {
	inline std::unique_ptr<identifier_expression> identifier(scope& scope, std::string name) {
		auto symbol = scope.lookup(name);
		return std::make_unique<identifier_expression>(symbol, identifier_expression::factory_tag_t{});
	}

	inline std::unique_ptr<identifier_expression> identifier(std::string name) {
		return identifier(rush::ensure_global_scope(), std::move(name));
	}
} // rush::ast::exprs

#endif // RUSH_AST_EXPRS_IDENTIFIER_HPP
