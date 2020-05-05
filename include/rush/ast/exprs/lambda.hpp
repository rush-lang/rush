#pragma once

#ifndef RUSH_AST_EXPRS_LAMBDA_HPP
#define RUSH_AST_EXPRS_LAMBDA_HPP

#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/function.hpp"

#include "rush/ast/context.hpp"

#include <memory>

namespace rush::ast {

	class lambda_expression : public ast::expression {
	public:
		lambda_expression(
         std::unique_ptr<ast::parameter_list> params,
			std::unique_ptr<ast::statement> body)
			: _params { std::move(params) }
         , _type { ast::types::undefined }
         , _rettype { ast::types::undefined }
			, _body { std::move(body) } {}

		ast::parameter_list const& parameters() const noexcept {
			return *_params;
		}

		ast::statement const& body() const noexcept {
			return *_body;
		}

      ast::type_ref return_type() const noexcept {
         return resolve_return_type();
      }

      virtual ast::type_ref result_type() const override {
         return _type;
		}

		virtual void accept(ast::visitor& v) const override {
			v.visit_lambda_expr(*this);
		}

      virtual void attach(ast::node&, ast::context& context) override {
         _params->attach(*this, context);
			_body->attach(*this, context);
         // use contextual function type.
         // IMPORTANT: In the case of deducing the functions return type,
         //            it must be performed after attaching the child
         //            nodes of the function to the context, so that
         //            ordinary non function types are properly established.
         _type = context.function_type(*this);
		}

      virtual void detach(ast::node&, ast::context& context) override {
         _params->detach(*this, context);
			_body->detach(*this, context);
         _type = types::undefined;
		}

	private:
		std::unique_ptr<ast::parameter_list> _params;
		std::unique_ptr<ast::statement> _body;
      mutable ast::type_ref _rettype;
      mutable ast::type_ref _type;

      struct lambda_expression_return_type_resolver : ast::visitor {
         ast::type_ref result = ast::types::undefined;
         virtual void visit_function_type(ast::function_type const& type) override {
            result = type.return_type();
         }
      };

      ast::type_ref resolve_return_type() const noexcept {
         return _rettype = (_rettype == ast::types::undefined
            ? rush::visit(_type, lambda_expression_return_type_resolver {}).result
            : _rettype);
      }
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
