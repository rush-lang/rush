#pragma once

#ifndef RUSH_AST_LIST_HPP
#define RUSH_AST_LIST_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/exprs/argument.hpp"
#include "rush/ast/decls/parameter.hpp"

#include <initializer_list>

namespace rush::ast::detail {
	template <typename T>
	class basic_list : public node {
	public:
		using container_type = std::vector<T>;

		using iterator = typename container_type::const_iterator;
		using const_iterator = typename container_type::const_iterator;

		const_iterator begin() const noexcept { return _elems.begin(); }
		const_iterator end() const noexcept { return _elems.end(); }
		const_iterator cbegin() const noexcept { return _elems.cbegin(); }
		const_iterator cend() const noexcept { return _elems.cend(); }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {

      }

	private:
		std::vector<std::unique_ptr<T>> _elems;
	};
}

namespace rush::ast {
	using argument_list = detail::basic_list<argument>;
	using parameter_list = detail::basic_list<parameter>;

	std::unique_ptr<argument_list> list(std::initializer_list<std::unique_ptr<argument>> args);
	std::unique_ptr<parameter_list> list(std::initializer_list<std::unique_ptr<parameter>> params);
}

#endif // RUSH_AST_LIST_HPP
