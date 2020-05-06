#pragma once

#ifndef RUSH_AST_DECLS_FUNCTION_HPP
#define RUSH_AST_DECLS_FUNCTION_HPP

#include "rush/ast/identifier.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/function.hpp"
#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/exprs/identifier.hpp"

#include "rush/ast/context.hpp"

namespace rush::ast::decls {
   std::unique_ptr<function_declaration> function(
      std::string,
      ast::type_ref,
      std::unique_ptr<parameter_list>,
      std::unique_ptr<statement>);
}

namespace rush::ast {
	class function_declaration : public declaration {
		struct factory_tag_t {};

      friend std::unique_ptr<function_declaration> decls::function(
         std::string,
         ast::type_ref,
         std::unique_ptr<parameter_list>,
         std::unique_ptr<statement>);

	public:
		function_declaration(
			std::string name,
         ast::type_ref return_type,
         std::unique_ptr<ast::parameter_list> params,
			std::unique_ptr<ast::statement> body,
			factory_tag_t)
         : _name { std::move(name) }
         , _type { types::undefined }
         , _return_type { std::move(return_type) }
         , _params { std::move(params) }
			, _body { std::move(body) } {}

      virtual std::string_view name() const noexcept override {
         return _name;
      }

      virtual ast::type_ref type() const noexcept override {
         return _type;
      }

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::function;
		}

      ast::type_ref return_type() const noexcept {
         return resolve_return_type();
      }

		ast::parameter_list const& parameters() const noexcept {
         return *_params;
		}

		ast::statement const& body() const noexcept {
			return *_body;
		}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_function_decl(*this);
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

         // use non-contextual function type.
         _type = context.function_type(
            context.tuple_type(_params->types()),
            _return_type);
      }

	private:
      std::string _name;
      ast::type_ref _type;
      ast::type_ref _return_type;
		std::unique_ptr<ast::statement> _body;
      std::unique_ptr<ast::parameter_list> _params;

      struct return_type_visitor : ast::visitor {
         ast::type_ref result_type = ast::types::undefined;
         virtual void visit_function_type(ast::function_type const& type) override {
            result_type = type.return_type();
         }
      };

      ast::type_ref resolve_return_type() const {
         return _return_type == types::undefined
              ? rush::visit(_type, return_type_visitor {}).result_type
              : _return_type;
      }
	};

	namespace decls {
		inline std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
               ast::types::undefined,
               decls::param_list(),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type_ref return_type,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
					std::move(return_type),
               decls::param_list(),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
               ast::types::undefined,
					std::move(params),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
         ast::type_ref return_type,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
            return std::make_unique<function_declaration>(
               std::move(name),
               std::move(return_type),
               std::move(params),
               std::move(body),
               function_declaration::factory_tag_t {});
			}
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_FUNCTION_HPP
