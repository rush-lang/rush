#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/sema/scope.hpp"
#include "rush/sema/symbol.hpp"

#include <string>

namespace rush::ast {
	class type : public node {
		friend type make_type(scope&, std::string name);
	public:
		std::size_t id() const noexcept { return _symbol->id(); }
		std::string name() const noexcept { return _symbol->name(); }

		using node::accept;
		virtual void accept(ast::visitor& v) const {
			v.visit_type(*this);
		}

	private:
		sema::symbol const* _symbol;
		type(sema::symbol const& s) : _symbol(&s) {}
	};

	inline bool operator == (type const& lhs, type const& rhs) {
		// fixme: implement id for sema::symbols.
		// return lhs.id() == rhs.id();
		return false;
	}

	inline bool operator != (type const& lhs, type const& rhs) {
		return !(lhs == rhs);
	}

	// known/built-in types
	extern type const undefined_type; // sentinal
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


	// \brief Returns the nearest base type of type.
	type base_of(type const& type);

	// \brief Returns the common type between two types,
	type intersection_of(type const&, type const&);

	// \brief Returns the common type between several types.
	template <typename... Types>
	inline type intersection_of(type const& first, type const& second, Types const&... rest) {
		static_assert(std::conjunction_v<std::is_same<type, Types>...>);
		return intersection_of(intersection_of(first, second), rest...);
	}
}

#endif // RUSH_AST_TYPE_HPP
