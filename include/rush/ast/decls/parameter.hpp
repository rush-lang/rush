#pragma once

#ifndef RUSH_AST_PARAMETER_HPP
#define RUSH_AST_PARAMETER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types/type.hpp"

namespace rush::ast {
	class parameter : public node {
	public:
		using node::accept;
      virtual ast::type_ref type() const = 0;
	};

	class named_parameter : public parameter {
	public:
		virtual ast::type_ref type() const noexcept override {
         return _type;
      }

		std::string name() {
         return _name;
      }

	private:
		ast::type_ref _type;
		std::string _name;
	};

	class unnamed_parameter : public parameter {
	public:
      virtual ast::type_ref type() const noexcept override {
         return _type;
      }

	private:
		ast::type_ref _type;
	};

	namespace decls {
		std::unique_ptr<unnamed_parameter> param(ast::type_ref type);
		std::unique_ptr<named_parameter> param(std::string name, ast::type_ref type);
	}
} // rush::ast

#endif // RUSH_AST_PARAMETER_HPP
