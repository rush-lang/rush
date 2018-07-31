#pragma once

#ifndef RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
#define RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/expression.hpp"
#include "rush/ast/literal.hpp"
#include "rush/ast/binary.hpp"

#include "rush/ast/visitor.hpp"

namespace rush {
	class parser_options;
	class lexical_analysis;

	class abstract_syntax_tree {
		friend abstract_syntax_tree parse(
			lexical_analysis const&,
			parser_options const&);

	public:
		void accept(ast::visitor&& v) const {
			accept(v);
		}

		void accept(ast::visitor& v) const {
			if (_root != nullptr)
				_root->accept(v);
		}

	private:
		std::unique_ptr<ast::node> _root;

		abstract_syntax_tree()
			: _root(nullptr) {}

		abstract_syntax_tree(std::unique_ptr<ast::node> root)
			: _root(std::move(root)) {}
	};

} // rush

#endif // RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
