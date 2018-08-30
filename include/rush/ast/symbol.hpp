#pragma once

#ifndef RUSH_AST_SYMBOL_HPP
#define RUSH_AST_SYMBOL_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/type.hpp"
#include "rush/sema/symbol.hpp"

#include <string>
#include <variant>

namespace rush::ast {
	class symbol : public node {
	public:
		symbol(std::string val) : _val(val) {}
		symbol(sema::symbol sym) : _val(sym) {}

		bool is_undefined() const noexcept {
			return std::holds_alternative<std::string>(_val)
				|| std::get<sema::symbol>(_val).is_undefined();
		}

		std::string name() const noexcept {
			return std::holds_alternative<sema::symbol>(_val)
				? std::get<sema::symbol>(_val).name();
				: std::get<std::string>(_val);
		}

		sema::symbol_kind type() const noexcept {
			return std::holds_alternative<sema::symbol>(_val)
				? std::get<sema::symbol>(_val).type()
				: sema::symbol_kind::undefined;
		}

		// used by the semantic analysis visitor to establish
		// the scope of named types after the syntax tree has
		// been constructed.
		void place(sema::scope&);

	private:
		std::variant<
			std::string,
			sema::symbol> _val;
	};
} // rush::ast

#endif // RUSH_AST_SYMBOL_HPP
