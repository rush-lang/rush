#pragma once

#ifndef RUSH_AST_TYPES_TUPLE_HPP
#define RUSH_AST_TYPES_TUPLE_HPP

#include "rush/extra/iterator_range.hpp"
#include "rush/ast/types/type.hpp"

#include <vector>

namespace rush::ast {
	class tuple_type;

	template <>
	struct type_traits<tuple_type> {
		static constexpr ast::type_kind kind() noexcept {
			return ast::type_kind::tuple;
		}
	};

	class tuple_type : public type {
	public:
		explicit tuple_type(std::vector<ast::type_ref> types)
			: _types { std::move(types) } {}

		std::vector<ast::type_ref> const& types() const noexcept {
			return _types;
		}

      virtual ast::type_kind kind() const override {
			return type_traits<tuple_type>::kind();
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_tuple_type(*this);
		}

      virtual void attach(ast::node&, ast::context&) override {}
      virtual void detach(ast::node&, ast::context&) override {}

	private:
		std::vector<ast::type_ref> _types;
	};
}

#endif // RUSH_AST_TYPES_TUPLE_HPP
