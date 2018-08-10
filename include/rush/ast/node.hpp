#pragma once

#ifndef RUSH_AST_NODE_HPP
#define RUSH_AST_NODE_HPP

namespace rush::ast {
	class visitor;

	// enum class statement_type : std::uint8_t;
	// enum class expression_type : std::uint8_t;
	// enum class declaration_type : std::uint8_t;


	class node {
	public:
		node() = default;
		virtual ~node() = default;

		virtual void accept(ast::visitor&) const = 0;
		virtual void accept(ast::visitor&& v) const { accept(v); }
	};
} // rush

#endif // RUSH_AST_NODE_HPP
