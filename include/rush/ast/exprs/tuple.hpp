#pragma once

#ifndef RUSH_AST_EXPRS_TUPLE_HPP
#define RUSH_AST_EXPRS_TUPLE_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/exprs/argument.hpp"
#include "rush/ast/list.hpp"

#include <algorithm>

namespace rush::ast {
	class tuple_expression;

	namespace exprs {
		std::unique_ptr<ast::tuple_expression> tuple(
			std::unique_ptr<ast::argument_list> args);
	}

	class tuple_expression : public ast::expression {
		struct factory_tag_t {};

		friend std::unique_ptr<ast::tuple_expression> exprs::tuple(
			std::unique_ptr<ast::argument_list> args);

	public:
		tuple_expression(
			std::unique_ptr<ast::argument_list> args,
			factory_tag_t)
			: _type { ast::types::undefined }
			, _args { std::move(args) } {}

		ast::argument_list const& arguments() const noexcept {
			return *_args;
		}

      virtual ast::type_ref result_type() const override {
			return _type;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_tuple_expr(*this);
		}

      virtual void attach(ast::node&, ast::context& ctx) override {
			// auto types = ast::type_list(_args)
			// _type = ctx.tuple_type(types);
			std::for_each(_args->begin(), _args->end(),
				[this, &ctx](auto& a) { a->attach(*this, ctx); });
		}

      virtual void detach(ast::node&, ast::context& ctx) override {
			// _type = ast::types::undefined;
			std::for_each(_args->begin(), _args->end(),
				[this, &ctx](auto& a) { a->detach(*this, ctx); });
		}

	private:
		ast::type_ref _type;
		std::unique_ptr<ast::argument_list> _args;
	};

	namespace exprs {
		inline std::unique_ptr<ast::tuple_expression> tuple(
			std::unique_ptr<ast::argument_list> args) {
				return std::make_unique<ast::tuple_expression>(
					std::move(args),
					tuple_expression::factory_tag_t {});
			}
	}
}

#endif // RUSH_AST_EXPRS_TUPLE_HPP
