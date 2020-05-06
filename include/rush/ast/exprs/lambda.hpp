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
         , _explicit_return_type { ast::types::undefined }
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
         return resolve_type();
		}

		virtual void accept(ast::visitor& v) const override {
			v.visit_lambda_expr(*this);
		}

      virtual void attach(ast::node&, ast::context& context) override {
         _type = context.function_type(*this);

         _params->attach(*this, context);
			_body->attach(*this, context);
		}

      virtual void detach(ast::node&, ast::context& context) override {
         _type = types::undefined;

         _params->detach(*this, context);
			_body->detach(*this, context);
		}

	private:
		std::unique_ptr<ast::parameter_list> _params;
		std::unique_ptr<ast::statement> _body;
      mutable ast::type_ref _explicit_return_type;
      mutable std::variant<
         ast::type_ref,
         ast::type_resolver*> _type;

      ast::type_ref resolve_type() const {
         struct type_visitor {
            std::variant<ast::type_ref, ast::type_resolver*>& _v;
            ast::type_ref operator ()(ast::type_ref type) { return type; }
            ast::type_ref operator ()(ast::type_resolver* r) { return std::get<ast::type_ref>(_v = r->resolve()); } };
         return std::visit(type_visitor { _type }, _type);
      }

      ast::type_ref resolve_return_type() const noexcept {
         struct lambda_expression_return_type_resolver : ast::visitor {
            ast::type_ref result_type = ast::types::undefined;
            virtual void visit_function_type(ast::function_type const& type) override
            { result_type = type.return_type(); } };

         return _explicit_return_type = (_explicit_return_type == ast::types::undefined
            ? rush::visit(result_type(), lambda_expression_return_type_resolver {}).result_type
            : _explicit_return_type);
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
