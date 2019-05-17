#pragma once

#ifndef RUSH_AST_DECLS_DECLARATION_HPP
#define RUSH_AST_DECLS_DECLARATION_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types/type.hpp"

#include <string>
#include <cstdint>

namespace rush::ast {
	enum class declaration_kind : std::uint8_t {
		constant,
		variable,
		function,
		alias,
		enum_,
		class_,
		struct_,
		concept,
		interface,
		extension,
		operator_,
	};

	class declaration : public node {
		declaration(declaration const&) = delete;
		void operator = (declaration const&) = delete;

	public:
      declaration(std::string name, ast::type type)
         : _name(std::move(name))
         , _type(std::move(type)) {}

      ast::type type() const noexcept {
			return _type;
		}

      std::string name() const noexcept {
			return _name;
		}

		virtual declaration_kind kind() const noexcept = 0;

   private:
      std::string _name;
      ast::type _type;
	};
}

#endif // RUSH_AST_DECLS_DECLARATION_HPP
