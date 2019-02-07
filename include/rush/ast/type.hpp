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

	// sentinal types
	extern type const error_type;

	// known/built-in types
	extern type const void_type;
	extern type const bool_type;
	extern type const byte_type;
	extern type const sbyte_type;
	extern type const short_type;
	extern type const ushort_type;
	extern type const int_type;
	extern type const uint_type;
	extern type const long_type;
	extern type const ulong_type;
	extern type const float_type;
	extern type const double_type;
	extern type const string_type;
	extern type const char_type;
}

#endif // RUSH_AST_TYPE_HPP
