#pragma once

#ifndef RUSH_AST_NODE_HPP
#define RUSH_AST_NODE_HPP

namespace rush::ast {
	class visitor;

	class node {
	public:
		node() = default;
		virtual ~node() = default;

		virtual void accept(ast::visitor&) const = 0;
		virtual void accept(ast::visitor&& v) const { accept(v); }
	};
} // rush

#endif // RUSH_AST_NODE_HPP
