#pragma once

#ifndef RUSH_AST_DECLARATION_HPP
#define RUSH_AST_DECLARATION_HPP

#include "rush/ast/node.hpp"

#include <cstdint>

namespace rush::ast {
	enum class declaration_kind : std::uint8_t {
		import,
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
	private:
		declaration(declaration const&) = delete;
		void operator = (declaration const&) = delete;

	public:
		declaration() = default;

		virtual declaration_kind kind() const noexcept = 0;
	};
}

#include "rush/ast/decls/storage.hpp"
#include "rush/ast/decls/function.hpp"

#endif // RUSH_AST_DECLARATION_HPP
