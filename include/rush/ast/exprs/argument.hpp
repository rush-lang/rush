#pragma once

#ifndef RUSH_AST_ARGUMENT_HPP
#define RUSH_AST_ARGUMENT_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/exprs/expression.hpp"

#include <vector>
#include <memory>

namespace rush::ast {
	class argument : public node {
	public:
      argument(std::unique_ptr<ast::expression> expr)
         : _expr { std::move(expr) } {}

      ast::expression const& expression() const {
         return *_expr;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         expression().accept(v);
      }

	private:
		std::unique_ptr<ast::expression> _expr;
	};

	class named_argument : public argument {
	public:
	private:
		std::string _name;
	};

	namespace exprs {
		inline std::unique_ptr<argument> arg(std::unique_ptr<expression> expr) {
         return std::make_unique<argument>(std::move(expr));
      }
		std::unique_ptr<named_argument> arg(std::string name, std::unique_ptr<expression> expr);
	}
} // rush::ast

#endif // RUSH_AST_ARGUMENT_HPP
