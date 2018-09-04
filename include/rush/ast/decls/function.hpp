#pragma once

#ifndef RUSH_AST_DECLS_FUNCTION_HPP
#define RUSH_AST_DECLS_FUNCTION_HPP

#include "rush/core/iterator_range.hpp"
#include "rush/ast/declaration.hpp"
#include "rush/ast/type.hpp"
#include "rush/ast/parameter.hpp"
#include "rush/sema/symbol.hpp"
#include "rush/sema/scope.hpp"

#include <vector>

namespace rush::ast {
	class statement;

	class function_declaration : public declaration {
		struct factory_tag_t {};
	public:
		using parameter_range = rush::iterator_range<
			typename std::vector<std::unique_ptr<parameter>>::iterator>;

		function_declaration(
			sema::symbol symbol,
			std::vector<std::unique_ptr<parameter>> params,
			std::unique_ptr<statement> body,
			factory_tag_t);

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::function;
		}

		ast::type return_type() const noexcept {
			return _symbol.type();
		}

		std::string name() const noexcept {
			return _symbol.name();
		}

		parameter_range parameters() const {
			// return { _params };
			throw std::runtime_error("not implemented");
		}

		statement const& body() const noexcept {
			return *_body;
		}

	private:
		sema::symbol _symbol;
		std::unique_ptr<statement> _body;
		std::vector<std::unique_ptr<parameter>> _params;
	};

	namespace decls {
		std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<statement> body);

		std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type return_type,
			std::unique_ptr<statement> body);

		std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body);

		std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type return_type,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body);

		std::unique_ptr<function_declaration> function(
			rush::scope&,
			std::string name,
			std::unique_ptr<statement> body);

		std::unique_ptr<function_declaration> function(
			rush::scope&,
			std::string name,
			ast::type return_type,
			std::unique_ptr<statement> body);

		std::unique_ptr<function_declaration> function(
			rush::scope&,
			std::string name,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body);

		std::unique_ptr<function_declaration> function(
			rush::scope&,
			std::string name,
			ast::type return_type,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body);
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_FUNCTION_HPP
