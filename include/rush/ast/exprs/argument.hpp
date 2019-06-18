#pragma once

#ifndef RUSH_AST_ARGUMENT_HPP
#define RUSH_AST_ARGUMENT_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/list.hpp"
#include "rush/ast/exprs/expression.hpp"

#include <vector>
#include <memory>
#include <string>
#include <string_view>

namespace rush::ast {
   class argument;
	using argument_list = detail::basic_list<argument>;

	class argument : public expression {
	public:
      argument(std::unique_ptr<ast::expression> expr)
         : _expr { std::move(expr) } {}

      ast::expression const& expression() const {
         return *_expr;
      }

      virtual ast::type_ref result_type() const override {
         return _expr->result_type();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_argument(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {
         _expr->attach(*this, context);
      }

      virtual void detach(ast::node&, ast::context& context) override {
         _expr->detach(*this, context);
      }

	private:
		std::unique_ptr<ast::expression> _expr;
	};

	class named_argument : public argument {
	public:
      named_argument(
         std::string name,
         std::unique_ptr<ast::expression> expr)
         : argument { std::move(expr) }
         , _name { std::move(name) } {}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_named_argument(*this);
      }

      std::string_view name() const noexcept {
         return _name;
      }

	private:
		std::string _name;
	};

	namespace exprs {
		inline std::unique_ptr<argument> arg(std::unique_ptr<expression> expr) {
         return std::make_unique<argument>(std::move(expr));
      }

		inline std::unique_ptr<named_argument> arg(std::string name, std::unique_ptr<expression> expr) {
         return std::make_unique<named_argument>(std::move(name), std::move(expr));
      }

      inline std::unique_ptr<ast::argument_list> arg_list() {
         return std::make_unique<argument_list>();
      }

      inline std::unique_ptr<argument_list> arg_list(std::vector<std::unique_ptr<argument>> args) {
         return std::make_unique<argument_list>(std::move(args));
      }
	} // rush::ast::exprs
} // rush::ast

#endif // RUSH_AST_ARGUMENT_HPP
