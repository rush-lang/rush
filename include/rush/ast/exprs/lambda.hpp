#pragma once

#ifndef RUSH_AST_EXPRS_LAMBDA_HPP
#define RUSH_AST_EXPRS_LAMBDA_HPP

#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/function.hpp"

#include <memory>

namespace rush::ast {

	class lambda_expression : public ast::expression {
	public:
		lambda_expression(
         ast::function_type fntype,
			std::unique_ptr<ast::statement> body)
			: _type { std::move(fntype) }
			, _body { std::move(body) } {}

		ast::parameter_list const& parameters() const noexcept {
			return _type.parameters();
		}

		ast::statement const& body() const noexcept {
			return *_body;
		}

      virtual ast::type_ref result_type() const override {
         _type.resolve_return_type(body());
         return { _type };
		}

		virtual void accept(ast::visitor& v) const override {
			v.visit_lambda_expr(*this);
		}

      virtual void attach(ast::node&, ast::context& ctx) override {
			_type.attach(*this, ctx);
			_body->attach(*this, ctx);
		}

      virtual void detach(ast::node&, ast::context& ctx) override {
			_type.detach(*this, ctx);
			_body->detach(*this, ctx);
		}

	private:
		std::unique_ptr<ast::statement> _body;
      ast::function_type _type;
	};

	namespace exprs {
		inline std::unique_ptr<lambda_expression> lambda(
			std::unique_ptr<ast::parameter_list> params,
			std::unique_ptr<ast::statement> body) {
				return std::make_unique<ast::lambda_expression>(
					ast::function_type {
                  ast::types::undefined,
                  std::move(params),
                  function_type::factory_tag_t {} },
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
