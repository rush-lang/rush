#pragma once

#ifndef RUSH_AST_PARAMETER_HPP
#define RUSH_AST_PARAMETER_HPP

#include "rush/ast/node.hpp"

namespace rush::ast {
	class parameter : public node {
	public:
		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			// v.visit_parameter(*this);
		}
	};

	class named_parameter : public parameter {
	public:
		ast::type type() { return _type; }
		std::string name() { return _name; }
	private:
		ast::type _type;
		std::string _name;
	};

	class unnamed_parameter : public parameter {
	public:
		ast::type type() { return _type; }
	private:
		ast::type _type;
	};

	namespace decls {
		std::unique_ptr<unnamed_parameter> param(ast::type type);
		std::unique_ptr<named_parameter> param(std::string name, ast::type type);
	}
} // rush::ast

#endif // RUSH_AST_PARAMETER_HPP
