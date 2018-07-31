#pragma once

#ifndef RUSH_AST_VARIABLE_HPP
#define RUSH_AST_VARIABLE_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/type.hpp"

#include <string>

namespace rush::ast {
	class declaration : public node {};

	class storage_declaration : public declaration {
	public:
		std::string name() const noexcept {
			return _name;
		}

		ast::type const& type() const noexcept {
			return *_type;
		}

		expression const& initializer() const noexcept {
			return *_init;
		}

	private:
		std::string _name;
		expression* _init;
		ast::type* _type;
	};

	class variable_declaration : public storage_declaration {
	public:
		virtual void accept(visitor&) const override;
	};

	class constant_declaration : public storage_declaration {
	public:
		virtual void accept(visitor&) const override;
	};
} // rush

#endif // RUSH_AST_VARIABLE_HPP
