#pragma once

#ifndef RUSH_AST_PARAMETER_HPP
#define RUSH_AST_PARAMETER_HPP

#include "rush/ast/node.hpp"
#include "rush/sema/symbol.hpp"

namespace rush::ast {
	class parameter : public node {
	public:
		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			// v.visit_parameter(*this);
		}
	private:
		sema::symbol _symbol;
	};

	class named_parameter : public parameter {
	public:
		std::string name() { return _symbol.name(); }
		ast::type type() { return { _symbol.type() }; }
	private:
		sema::symbol _symbol;
	};

	class unnamed_parameter : public parameter {
	public:
		ast::type type() { return { _symbol }; }
	private:
		sema::symbol _symbol;
	};

	namespace decls {
		std::unique_ptr<unnamed_parameter> param(ast::type type);
		std::unique_ptr<named_parameter> param(std::string name, ast::type type);
	}
} // rush::ast

#endif // RUSH_AST_PARAMETER_HPP
