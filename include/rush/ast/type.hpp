#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace rush::ast {
	struct type_hash {};

	class type : public node {
		friend type& known_type(std::string name, bool builtin);
		friend type& unknown_type(std::string name);
	public:
		// \brief Returns true if the type is a known type; false otherwise.
		bool is_known() const noexcept;

		// \brief Returns true if the type is a built-in (e.g. int); false otherwise.
		bool is_builtin() const noexcept;

		// \brief Returns true if the type is an optional type (e.g. int?); false otherwise.
		bool is_optional() const noexcept;

		std::size_t id() const noexcept {
			return _hash;
		}

		std::string name() const noexcept {
			return _name;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const {
			v.visit_type(*this);
		}

	private:
		std::string _name;
		std::size_t _hash;
	};

	inline bool operator == (type const& lhs, type const& rhs) {
		return lhs.id() == rhs.id();
	}

	inline bool operator != (type const& lhs, type const& rhs) {
		return !(lhs == rhs);
	}

	// known/built-in types
	extern type const none_type; // sentinal
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
