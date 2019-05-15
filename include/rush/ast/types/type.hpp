#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace rush::ast {
	class type : public node {
		friend type make_primitive_type(std::string const& name);

	public:
		type(std::string const& s) : _name(s) {}

		// std::size_t id() const noexcept {
		// 	return _name.id();
		// }

		std::string name() const noexcept {
			return _name;
		}

		// sema::symbol symbol() const noexcept {
		// 	return _name;
		// }

		// bool is_undefined() const noexcept {
		// 	return _name.is_undefined();
		// }

		using node::accept;
		virtual void accept(ast::visitor& v) const {
			v.visit_type(*this);
		}

	private:
		std::string _name;
	};

	inline bool operator == (type const& lhs, type const& rhs) {
		return lhs.name() == rhs.name();
	}

	inline bool operator != (type const& lhs, type const& rhs) {
		return !(lhs == rhs);
	}
}

#endif // RUSH_AST_TYPE_HPP
