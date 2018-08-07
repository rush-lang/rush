#pragma once

#ifndef RUSH_AST_VARIABLE_HPP
#define RUSH_AST_VARIABLE_HPP

#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/type.hpp"

#include <string>

namespace rush::ast {

	class storage_declaration : public declaration {
	public:
		std::string name() const noexcept {
			return _name;
		}

		ast::type type() const noexcept {
			return _type == nullptr
				? _init->result_type()
				: *_type;
		}

		expression const& initializer() const noexcept {
			return *_init;
		}

	protected:
		storage_declaration(std::string name, std::unique_ptr<expression> init)
			: _name { std::move(name) }
			, _init { std::move(init) }
			, _type { nullptr } {}

		storage_declaration(std::string name, ast::type const& type, std::unique_ptr<expression> init)
			: _name { std::move(name) }
			, _init { std::move(init) }
			, _type { std::addressof(type) } {}

	private:
		std::string _name;
		std::unique_ptr<expression> _init;
		ast::type const* const _type;
	};

	class constant_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<constant_declaration>
			constant_decl(std::string, ast::type, std::unique_ptr<expression>);

	public:
		constant_declaration(std::string name, ast::type type, std::unique_ptr<expression> init, factory_tag_t)
			: storage_declaration { std::move(name), type, std::move(init) } {}

		virtual void accept(ast::visitor& vis) const override {
			vis.visit_constant_decl(*this);
		}
	};

	class variable_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<variable_declaration>
			variable_decl(std::string, ast::type, std::unique_ptr<expression>);

	public:
		variable_declaration(std::string name, ast::type type, std::unique_ptr<expression> init, factory_tag_t)
			: storage_declaration { std::move(name), type, std::move(init) } {}

		virtual void accept(ast::visitor& vis) const override {
			vis.visit_variable_decl(*this);
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

	inline std::unique_ptr<variable_declaration> variable_decl(
		std::string name,
		ast::type type,
		std::unique_ptr<expression> init
	) {
		return std::make_unique<variable_declaration>(
			std::move(name),
			type,
			std::move(init),
			variable_declaration::factory_tag_t{});
	}

	inline std::unique_ptr<variable_declaration> variable_decl(
		std::string name,
		std::unique_ptr<expression> init
	) {
		return variable_decl(
			std::move(name),
			init->result_type(),
			std::move(init));
	}
} // rush::ast

#endif // RUSH_AST_VARIABLE_HPP
