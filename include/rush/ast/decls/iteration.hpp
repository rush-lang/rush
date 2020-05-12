#pragma once

#ifndef RUSH_AST_DECLS_ITERATION_VARIABLE_HPP
#define RUSH_AST_DECLS_ITERATION_VARIABLE_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/decls/storage.hpp"

namespace rush::ast {
   class iteration_variable_declaration;

	namespace decls {
		std::unique_ptr<iteration_variable_declaration> iteration_variable(
			std::string name,
			ast::type_ref type,
			std::unique_ptr<expression> init);
	}

   class iteration_variable_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<iteration_variable_declaration>
			decls::iteration_variable(std::string, ast::type_ref, std::unique_ptr<expression>);

	public:
		iteration_variable_declaration(
         std::string name,
         ast::type_ref type,
         std::unique_ptr<expression> init,
         factory_tag_t)
			: storage_declaration {
				std::move(name),
				std::move(type),
				std::move(init)
			} {}

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::iteration_variable;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_iteration_variable_decl(*this);
		}
	};
}

#endif // RUSH_AST_DECLS_ITERATION_VARIABLE_HPP
