#pragma once

#ifndef RUSH_AST_DECLS_FUNCTION_HPP
#define RUSH_AST_DECLS_FUNCTION_HPP

#include "rush/core/iterator_range.hpp"
#include "rush/ast/list.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/exprs/argument.hpp"

#include <vector>

namespace rush::ast {
	class statement;

	class function_declaration : public declaration {
		struct factory_tag_t {};
	public:
		using parameter_range = rush::iterator_range<
			typename std::vector<std::unique_ptr<parameter>>::iterator>;

		function_declaration(
			std::string name,
			ast::type return_type,
			std::vector<std::unique_ptr<parameter>> params,
			std::unique_ptr<statement> body,
			factory_tag_t)
         : declaration {
            std::move(name),
            std::move(return_type) }
			, _params { std::move(params) }
			, _body { std::move(body) } {}

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::function;
		}

      ast::type return_type() const noexcept {
			return this->type();
		}

		parameter_range parameters() const {
			// return { _params };
			throw std::runtime_error("not implemented");
		}

		statement const& body() const noexcept {
			return *_body;
		}

	private:
		std::unique_ptr<statement> _body;
		std::vector<std::unique_ptr<parameter>> _params;
	};

	namespace decls {
		inline std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type return_type,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
					std::move(return_type),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
					std::move(params),
					std::move(body));
			}

		inline std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type return_type,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
				return decls::function(
					std::move(name),
					std::move(return_type),
					std::move(params),
					std::move(body));
			}
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_FUNCTION_HPP
