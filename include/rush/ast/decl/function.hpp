#pragma once

#ifndef RUSH_AST_DECL_FUNCTION_HPP
#define RUSH_AST_DECL_FUNCTION_HPP

#include "rush/ast/declaration.hpp"

#include <vector>

namespace rush::ast {
	class parameter : public node {

	};

	class function_declaration : public declaration {
		struct factory_tag_t {};
	public:
		function_declaration(std::string name, std::vector<parameter> params)
			: _name(std::move(name))
			, _params(std::move(params)) {}

		ast::type return_type() const noexcept {
			return ast::none_type;
			// return _return_type != nullptr
			// 	? *_return_type
			// 	: evaluate_return_type();
		}

		std::string const& name() const noexcept {
			return _name;
		}

		std::vector<parameter> const& parameters() const noexcept {
			return _params;
		}

	private:
		std::string _name;
		std::vector<parameter> _params;
	};

	std::unique_ptr<function_declaration> function_decl(std::string name);
} // rush::ast

#endif // RUSH_AST_DECL_FUNCTION_HPP
