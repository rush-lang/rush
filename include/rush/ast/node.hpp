#pragma once

#ifndef RUSH_AST_NODE_HPP
#define RUSH_AST_NODE_HPP

namespace rush::ast {
	class visitor;

	class node {
	protected:
		node() = default;
		virtual ~node() = default;
		node(node const&) = delete;
		void operator = (node const&) = delete;

	public:
		virtual void accept(ast::visitor&) const = 0;
	};
} // rush

#endif // RUSH_AST_NODE_HPP
