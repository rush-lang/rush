#pragma once

#ifndef RUSH_AST_LIST_HPP
#define RUSH_AST_LIST_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/exprs/expression.hpp"

#include <cstddef>
#include <vector>
#include <type_traits>

namespace rush::ast::detail {
	template <typename T>
	class basic_list : public node {
	public:
		using container_type = std::vector<std::unique_ptr<T>>;

      using value_type = typename container_type::value_type;
      using const_pointer = typename container_type::const_pointer;
      using const_reference = typename container_type::const_reference;
      using size_type = std::size_t;

		using iterator = typename container_type::const_iterator;
		using const_iterator = typename container_type::const_iterator;

      basic_list() {}

      basic_list(container_type elems)
         : _elems { std::move(elems) } {}

      bool empty() const noexcept {
         return _elems.empty();
      }

      size_type count() const noexcept {
         return _elems.size();
      }

      const_reference front() const noexcept { return _elems.front(); }
      const_reference back() const noexcept { return _elems.back(); }

      const_reference first() const noexcept {
         return _elems.front();
      }

      const_reference last() const noexcept {
         return _elems.back();
      }

		const_iterator begin() const noexcept {
         return _elems.begin();
      }

		const_iterator end() const noexcept {
         return _elems.end();
      }

		const_iterator cbegin() const noexcept {
         return _elems.cbegin();
      }

		const_iterator cend() const noexcept {
         return _elems.cend();
      }

      std::vector<ast::type_ref> result_types() const {
         auto types = std::vector<ast::type_ref> {};
         std::transform(this->begin(), this->end(),
            std::back_inserter(types), [](auto& x) { return x->result_type(); });
         return std::move(types);
      }

      std::vector<ast::type_ref> types() const {
         auto types = std::vector<ast::type_ref> {};
         std::transform(this->begin(), this->end(),
            std::back_inserter(types), [](auto& x) { return x->type(); });
         return std::move(types);
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
         std::for_each(_elems.begin(), _elems.end(), [&v](auto& p) { p->accept(v); });
      }

      virtual void attach(ast::node& parent, ast::context& context) override {
         std::for_each(_elems.begin(), _elems.end(),
            [&parent, &context](auto& elem) { elem->attach(parent, context); });
      }

      virtual void detach(ast::node& parent, ast::context& context) override {
         std::for_each(_elems.begin(), _elems.end(),
            [&parent, &context](auto& elem) { elem->detach(parent, context); });
      }

	private:
		container_type _elems;
	};
} // rush::ast::detail

#endif // RUSH_AST_LIST_HPP
