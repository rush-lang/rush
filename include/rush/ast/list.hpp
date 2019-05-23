#pragma once

#ifndef RUSH_AST_LIST_HPP
#define RUSH_AST_LIST_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/exprs/argument.hpp"
#include "rush/ast/decls/parameter.hpp"

#include <cstddef>
#include <vector>

namespace rush::ast::detail {
	template <typename T>
	class basic_list : public node {
	public:
		using container_type = std::vector<T>;

      using value_type = typename container_type::value_type;
      using const_pointer = typename container_type::const_pointer;
      using const_reference = typename container_type::const_reference;
      using size_type = std::size_t;

		using iterator = typename container_type::const_iterator;
		using const_iterator = typename container_type::const_iterator;

      basic_list() {}

      basic_list(container_type elems)
         : _elems { std::move(elems) } {}

      basic_list(std::vector<std::unique_ptr<T>> elems) {}

      bool empty() const noexcept {
         return _elems.empty();
      }

      size_type count() const noexcept {
         return _elems.size();
      }

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

		using node::accept;
		virtual void accept(ast::visitor& v) const override {

      }

	private:
		container_type _elems;
	};
} // rush::ast::detail

namespace rush::ast {
	using argument_list = detail::basic_list<argument>;
	using parameter_list = detail::basic_list<parameter>;
} // rush::ast

namespace rush::ast::exprs {

   inline std::unique_ptr<ast::argument_list> arg_list() {
      return std::make_unique<argument_list>();
   }

   inline std::unique_ptr<argument_list> arg_list(std::vector<std::unique_ptr<argument>> args) {
      return std::make_unique<argument_list>(std::move(args));
   }
} // rush::ast::exprs

namespace rush::ast::decls {

   inline std::unique_ptr<parameter_list> param_list() {
      return std::make_unique<parameter_list>();
   }

	inline std::unique_ptr<parameter_list> param_list(std::vector<std::unique_ptr<parameter>> params) {
      return std::make_unique<parameter_list>(std::move(params));
   }
} // rush::ast::decls

#endif // RUSH_AST_LIST_HPP
