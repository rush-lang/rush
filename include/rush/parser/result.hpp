#pragma once

#ifndef RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
#define RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>

namespace rush {
	class parser_options;
	class lexical_analysis;

	class syntax_analysis {
		friend syntax_analysis parse(
			lexical_analysis const&,
			parser_options const&);

	public:
		ast::node* ast() noexcept {
			return _root.get();
		}

		ast::node const* ast() const noexcept {
			return _root.get();
		}

	private:
		std::unique_ptr<ast::node> _root;

		syntax_analysis()
			: _root(nullptr) {}

		syntax_analysis(std::unique_ptr<ast::node> ast)
			: _root(std::move(ast)) {}
	};

} // rush

#endif // RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
