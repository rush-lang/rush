#pragma once

#ifndef RUSH_AST_DECL_CONSTANT_HPP
#define RUSH_AST_DECL_CONSTANT_HPP

#include "rush/ast/decl/storage.hpp"

namespace rush::ast {
	class constant_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<constant_declaration>
			constant_decl(std::string, ast::type, std::unique_ptr<expression>);

	public:
		constant_declaration(std::string name, ast::type type, std::unique_ptr<expression> init, factory_tag_t)
			: storage_declaration { std::move(name), type, std::move(init) } {}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_constant_decl(*this);
		}
	};

	inline std::unique_ptr<constant_declaration> constant_decl(
		std::string name,
		ast::type type,
		std::unique_ptr<expression> init
	) {
		return std::make_unique<constant_declaration>(
			std::move(name),
			type,
			std::move(init),
			constant_declaration::factory_tag_t{});
	}

	inline std::unique_ptr<constant_declaration> constant_decl(
		std::string name,
		std::unique_ptr<expression> init
	) {
		return constant_decl(
			std::move(name),
			init->result_type(),
			std::move(init));
	}
} // rush::ast

#endif // RUSH_AST_DECL_CONSTANT_HPP
