#pragma once

#ifndef RUSH_AST_EXPRS_LAMBDA_HPP
#define RUSH_AST_EXPRS_LAMBDA_HPP

#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/list.hpp"
#include <memory>

namespace rush::ast {

	class lambda_expression : public ast::expression {
	public:
		lambda_expression(
			std::unique_ptr<ast::parameter_list> params,
			std::unique_ptr<ast::statement> body)
			: _params { std::move(params) }
			, _body { std::move(body) } {}

		ast::parameter_list const& parameters() const noexcept {
			return *_params;
		}

		ast::statement const& body() const noexcept {
			return *_body;
		}

      virtual ast::type_ref result_type() const override {
			return types::undefined;
		}

		virtual void accept(ast::visitor& v) const override {
			v.visit_lambda_expr(*this);
		}

      virtual void attach(ast::node&, ast::context& ctx) override {
			_params->attach(*this, ctx);
			_body->attach(*this, ctx);
		}

      virtual void detach(ast::node&, ast::context& ctx) override {
			_params->detach(*this, ctx);
			_body->detach(*this, ctx);
		}

	private:
		std::unique_ptr<ast::parameter_list> _params;
		std::unique_ptr<ast::statement> _body;
	};

	namespace exprs {
		inline std::unique_ptr<lambda_expression> lambda(
			std::unique_ptr<ast::parameter_list> params,
			std::unique_ptr<ast::statement> body) {
				return std::make_unique<ast::lambda_expression>(
					std::move(params),
					std::move(body));
			}

		inline std::unique_ptr<lambda_expression> lambda(
			std::unique_ptr<ast::statement> body) {
				return lambda(
					decls::param_list(),
					std::move(body));
			}
	}
}

#endif // RUSH_AST_EXPRS_LAMBDA_HPP
