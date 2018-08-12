#pragma once

#ifndef RUSH_AST_IDENTIFIER_HPP
#define RUSH_AST_IDENTIFIER_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/sema/symbol.hpp"

#include <string>

namespace rush::ast {
	// \brief Represents an identifier within an expression
	//        such as a variable, or constant.
	class identifier_expression : public expression {
		struct factory_tag_t {};
		friend std::unique_ptr<identifier_expression>
			identifier_expr(scope&, std::string);

	public:
		identifier_expression(sema::symbol symbol, factory_tag_t) noexcept
			: _symbol(symbol) {}

		std::string name() const {
			return _symbol.name();
		}

		bool is_undefined() const noexcept {
			return undefined_type == result_type();
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

	inline std::unique_ptr<identifier_expression> identifier_expr(scope& scope, std::string name) {
		auto symbol = scope.insert(sema::make_identifier_entry(std::move(name)));
		return std::make_unique<identifier_expression>(symbol, identifier_expression::factory_tag_t{});
	}

	inline std::unique_ptr<identifier_expression> identifier_expr(std::string name) {
		return identifier_expr(rush::ensure_global_scope(), std::move(name));
	}
} // rush

#endif // RUSH_AST_IDENTIFIER_HPP
