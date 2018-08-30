#pragma once

#ifndef RUSH_AST_DECL_STORAGE_HPP
#define RUSH_AST_DECL_STORAGE_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/type.hpp"

#include "rush/sema/symbol.hpp"

#include <string>

namespace rush::ast {

	class storage_declaration : public declaration {
	public:
		std::string name() const noexcept {
			return _symbol.name();
		}

		ast::type type() const noexcept {
			auto static_type = ast::type { _symbol.type() };
			return static_type.is_undefined()
				? _init->result_type()
				: static_type;
		}

		expression const& initializer() const noexcept {
			return *_init;
		}

	protected:
		storage_declaration(sema::symbol symbol, std::unique_ptr<expression> init)
			: _symbol { symbol }
			, _init { std::move(init) } {}

	private:
		sema::symbol _symbol;
		std::unique_ptr<expression> _init;
	};
} // rush::ast


#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

#endif // RUSH_AST_DECL_STORAGE_HPP
