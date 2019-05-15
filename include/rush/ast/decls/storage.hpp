#pragma once

#ifndef RUSH_AST_DECLS_STORAGE_HPP
#define RUSH_AST_DECLS_STORAGE_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace rush::ast {
	class storage_declaration : public declaration {
	public:
		std::string name() const noexcept {
			return _name;
		}

		expression* initializer() const noexcept {
			return _init.get();
		}

		ast::type type() const noexcept {
			return _type;
		}

	protected:
		storage_declaration(std::string name, ast::type type, std::unique_ptr<expression> init)
			: _name { std::move(name) }
			, _type { std::move(type) }
			, _init { std::move(init) } {}

	private:
		ast::type _type;
		std::string _name;
		std::unique_ptr<expression> _init;
	};
} // rush::ast


#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

#endif // RUSH_AST_DECLS_STORAGE_HPP
