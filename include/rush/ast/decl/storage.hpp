#pragma once

#ifndef RUSH_AST_DECL_STORAGE_HPP
#define RUSH_AST_DECL_STORAGE_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/type.hpp"

#include <string>

namespace rush::ast {

	class storage_declaration : public declaration {
	public:
		std::string name() const noexcept {
			return _name;
		}

		ast::type type() const noexcept {
			return _type == nullptr
				? _init->result_type()
				: *_type;
		}

		expression const& initializer() const noexcept {
			return *_init;
		}

	protected:
		storage_declaration(std::string name, std::unique_ptr<expression> init)
			: _name { std::move(name) }
			, _init { std::move(init) }
			, _type { nullptr } {}

		storage_declaration(std::string name, ast::type const& type, std::unique_ptr<expression> init)
			: _name { std::move(name) }
			, _init { std::move(init) }
			, _type { std::addressof(type) } {}

	private:
		std::string _name;
		std::unique_ptr<expression> _init;
		ast::type const* const _type;
	};
} // rush::ast


#include "rush/ast/decl/constant.hpp"
#include "rush/ast/decl/variable.hpp"

#endif // RUSH_AST_DECL_STORAGE_HPP
