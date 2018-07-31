#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"

namespace rush::ast {
	class type : public node {
	public:
		bool is_known() const noexcept;
		bool is_builtin() const noexcept;

		virtual void accept(ast::visitor&) const;
	};

	// known/built-in types
	extern type const& void_type;
	extern type const& bool_type;
	extern type const& byte_type;
	extern type const& sbyte_type;
	extern type const& short_type;
	extern type const& ushort_type;
	extern type const& int_type;
	extern type const& uint_type;
	extern type const& long_type;
	extern type const& ulong_type;
	extern type const& float_type;
	extern type const& double_type;
	extern type const& string_type;
	extern type const& char_type;
}

#endif // RUSH_AST_TYPE_HPP
