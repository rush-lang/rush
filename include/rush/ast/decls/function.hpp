#pragma once

#ifndef RUSH_AST_DECLS_FUNCTION_HPP
#define RUSH_AST_DECLS_FUNCTION_HPP

#include "rush/ast/identifier.hpp"
#include "rush/ast/list.hpp"
#include "rush/ast/types/function.hpp"
#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/list.hpp"
#include "rush/ast/exprs/argument.hpp"

#include <vector>

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
         ast::function_type fntype,
			std::unique_ptr<ast::statement> body,
			factory_tag_t)
         : _name { std::move(name) }
         , _type { std::move(fntype) }
			, _body { std::move(body) } {}

      virtual std::string name() const noexcept override {
         return _name;
      }

      virtual ast::type_ref type() const noexcept override {
         return { _type };
      }

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::function;
		}

      ast::type_ref return_type() const noexcept {
			return _type.return_type();
		}

		ast::parameter_list const& parameters() const {
         return _type.parameters();
		}

		ast::statement const& body() const noexcept {
			return *_body;
		}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_function_decl(*this);
      }

	private:
      std::string _name;
      ast::function_type _type;
		std::unique_ptr<ast::statement> _body;
	};

	namespace decls {
		inline std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
               types::int_type,
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
               types::int_type,
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
               ast::function_type {
                  std::move(return_type),
                  std::move(params),
                  function_type::factory_tag_t {} },
               std::move(body),
               function_declaration::factory_tag_t {});
			}
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_FUNCTION_HPP
